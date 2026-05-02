use std::collections::HashSet;
use std::path::PathBuf;
use std::sync::Arc;
use std::time::Duration;

use axum::extract::State;
use axum::http::StatusCode;
use axum::response::{IntoResponse, Json, Response};
use axum::routing::{get, post};
use axum::Router;
use serde::{Deserialize, Serialize};
use tokio::io::{AsyncBufReadExt, AsyncWriteExt, BufReader};
use tokio::process::{Child, Command};
use tokio::sync::Mutex;
use tower_http::cors::CorsLayer;
use tower_http::services::ServeDir;
use tracing::Instrument;

mod observability;

// Per-request middleware: opens an `http_request` span with `session.id`
// declared so the field is recordable, then runs the inner handler chain
// inside that span via `Instrument`. Every handler span downstream becomes
// a child of this one, so Honeycomb sees one trace per request rooted at
// the HTTP entry point.
async fn session_id_middleware(
    req: axum::extract::Request,
    next: axum::middleware::Next,
) -> axum::response::Response {
    let session_id = req
        .headers()
        .get("X-Session-Id")
        .and_then(|v| v.to_str().ok())
        .unwrap_or("")
        .to_owned();
    let method = req.method().clone();
    let uri = req.uri().clone();
    let span = tracing::info_span!(
        "http_request",
        "session.id" = %session_id,
        "http.method" = %method,
        "http.target" = %uri.path(),
    );
    next.run(req).instrument(span).await
}

const OPCIONS_MARKER: &str = "OPCIONS:";
const INIT_MARKER: &str = "Inserta k";

fn menu_lines() -> HashSet<&'static str> {
    [
        "crea_especie",
        "obtener_gen",
        "distancia",
        "elimina_especie",
        "existe_especie",
        "lee_cjt_especies",
        "imprime_cjt_especies",
        "tabla_distancias",
        "inicialitza_clusters",
        "ejecuta_paso_wpgma",
        "imprime_cluster",
        "imprime_arbol_filogenetico",
        "fin",
    ]
    .into_iter()
    .collect()
}

struct ProcessManager {
    child: Option<Child>,
    stdin: Option<tokio::process::ChildStdin>,
    lines: Arc<Mutex<Vec<String>>>,
    k_value: Option<i32>,
}

impl ProcessManager {
    fn new() -> Self {
        Self {
            child: None,
            stdin: None,
            lines: Arc::new(Mutex::new(Vec::new())),
            k_value: None,
        }
    }

    fn alive(&self) -> bool {
        self.child
            .as_ref()
            .map(|c| c.id().is_some())
            .unwrap_or(false)
    }

    #[tracing::instrument(skip_all, fields(child.k = k, child.pid = tracing::field::Empty))]
    async fn start(&mut self, k: i32, exe_path: &PathBuf, cwd: &PathBuf) -> Result<(), String> {
        self.stop().await;
        self.k_value = Some(k);
        self.lines.lock().await.clear();

        let mut child = Command::new(exe_path)
            .stdin(std::process::Stdio::piped())
            .stdout(std::process::Stdio::piped())
            .stderr(std::process::Stdio::null())
            .current_dir(cwd)
            .spawn()
            .map_err(|e| {
                tracing::error!(error.kind = "spawn", error.message = %e, "child spawn failed");
                format!("Failed to start process: {e}")
            })?;

        if let Some(pid) = child.id() {
            tracing::Span::current().record("child.pid", pid);
        }

        let stdout = child.stdout.take().ok_or("No stdout")?;
        let stdin = child.stdin.take().ok_or("No stdin")?;

        self.child = Some(child);
        self.stdin = Some(stdin);

        let lines = self.lines.clone();
        tokio::spawn(async move {
            let reader = BufReader::new(stdout);
            let mut reader_lines = reader.lines();
            while let Ok(Some(line)) = reader_lines.next_line().await {
                lines.lock().await.push(line);
            }
        });

        self.send_raw(&format!("{k}\n")).await;
        self.collect_until_opcions().await;
        Ok(())
    }

    async fn stop(&mut self) {
        if self.alive() {
            let _ = self.send_raw("fin\n").await;
            if let Some(ref mut child) = self.child {
                let _ = tokio::time::timeout(Duration::from_secs(3), child.wait()).await;
                let _ = child.kill().await;
            }
        }
        self.child = None;
        self.stdin = None;
    }

    #[tracing::instrument(
        skip_all,
        fields(
            child.command = command,
            child.argv_len = args.len(),
            child.input_bytes = tracing::field::Empty,
            child.output_bytes = tracing::field::Empty,
            child.lines = tracing::field::Empty,
            child.elapsed_ms = tracing::field::Empty,
        )
    )]
    async fn send_command(&mut self, command: &str, args: &[String]) -> Result<String, String> {
        let span = tracing::Span::current();
        if !self.alive() {
            tracing::error!(error.kind = "process_dead", "child not running");
            return Err("Process not running. Call POST /api/init first.".into());
        }

        let input = if command == "lee_cjt_especies" && !args.is_empty() {
            let n: usize = args[0].parse().unwrap_or(0);
            let species = &args[1..];
            let mut s = format!("lee_cjt_especies\n{n}\n");
            for sp in species {
                s.push_str(sp);
                s.push('\n');
            }
            s
        } else if args.is_empty() {
            format!("{command}\n")
        } else {
            format!("{command} {}\n", args.join(" "))
        };

        span.record("child.input_bytes", input.len());

        let start = std::time::Instant::now();
        self.drain_buffer().await;
        self.send_raw(&input).await;
        let raw = self.collect_until_opcions().await;
        let elapsed_ms = start.elapsed().as_millis() as u64;
        let stripped = strip_echo(&raw);

        span.record("child.output_bytes", stripped.len());
        span.record("child.lines", stripped.lines().count());
        span.record("child.elapsed_ms", elapsed_ms);

        Ok(stripped)
    }

    async fn send_raw_and_collect(&mut self, text: &str) -> String {
        self.drain_buffer().await;
        self.send_raw(text).await;
        let raw = self.collect_until_opcions().await;
        strip_echo(&raw)
    }

    async fn drain_buffer(&self) {
        self.lines.lock().await.clear();
    }

    async fn send_raw(&mut self, text: &str) {
        if let Some(ref mut stdin) = self.stdin {
            let _ = stdin.write_all(text.as_bytes()).await;
            let _ = stdin.flush().await;
        }
    }

    async fn collect_until_opcions(&self) -> String {
        let menu = menu_lines();
        let expected_menu_count = menu.len() + 1;
        let deadline = tokio::time::Instant::now() + Duration::from_secs(5);
        let mut collected: Vec<String> = Vec::new();
        let mut seen_opcions = false;
        let mut menu_count: usize = 0;
        let mut cursor: usize = 0;

        while tokio::time::Instant::now() < deadline {
            {
                let buf = self.lines.lock().await;
                while cursor < buf.len() {
                    let line = &buf[cursor];
                    cursor += 1;

                    if line == OPCIONS_MARKER {
                        seen_opcions = true;
                        menu_count = 1;
                        continue;
                    }
                    if seen_opcions {
                        let trimmed = line.trim();
                        if menu.contains(trimmed) {
                            menu_count += 1;
                            if menu_count >= expected_menu_count {
                                return collected.join("\n");
                            }
                            continue;
                        } else {
                            seen_opcions = false;
                            menu_count = 0;
                        }
                    }
                    if line == INIT_MARKER {
                        continue;
                    }
                    collected.push(line.clone());
                }
            }
            tokio::time::sleep(Duration::from_millis(20)).await;
        }
        collected.join("\n")
    }
}

fn strip_echo(raw: &str) -> String {
    let lines: Vec<&str> = raw
        .lines()
        .filter(|l| !l.starts_with("# "))
        .collect();
    let trimmed: Vec<&str> = lines
        .into_iter()
        .collect::<Vec<_>>();
    let start = trimmed.iter().position(|l| !l.trim().is_empty()).unwrap_or(0);
    let end = trimmed
        .iter()
        .rposition(|l| !l.trim().is_empty())
        .map(|i| i + 1)
        .unwrap_or(0);
    trimmed[start..end].join("\n")
}

type AppState = Arc<Mutex<ProcessManager>>;

#[derive(Clone)]
struct Ctx {
    pm: AppState,
    exe_path: PathBuf,
    cwd: PathBuf,
}

#[derive(Deserialize)]
struct InitRequest {
    k: i32,
}

#[derive(Deserialize)]
struct CommandRequest {
    command: String,
    #[serde(default)]
    args: Vec<String>,
}

#[derive(Deserialize)]
struct SpeciesEntry {
    id: String,
    gen: String,
}

#[derive(Deserialize)]
struct ReadSpeciesRequest {
    species: Vec<SpeciesEntry>,
}

#[derive(Serialize)]
struct StatusResponse {
    alive: bool,
    k: Option<i32>,
}

#[derive(Serialize)]
struct InitResponse {
    status: String,
    k: i32,
}

#[derive(Serialize)]
struct CommandResponse {
    output: String,
    lines: Vec<String>,
}

struct AppError(StatusCode, String);

impl IntoResponse for AppError {
    fn into_response(self) -> Response {
        (self.0, Json(serde_json::json!({"detail": self.1}))).into_response()
    }
}

#[tracing::instrument(
    skip_all,
    fields(
        request.k = req.k,
        process.restarted = tracing::field::Empty,
        init.outcome = tracing::field::Empty,
    )
)]
async fn api_init(
    State(ctx): State<Ctx>,
    Json(req): Json<InitRequest>,
) -> Result<Json<InitResponse>, AppError> {
    let span = tracing::Span::current();
    if req.k < 1 {
        span.record("init.outcome", "invalid_k");
        tracing::error!(error.kind = "validation", error.message = "k must be >= 1");
        return Err(AppError(StatusCode::BAD_REQUEST, "k must be >= 1".into()));
    }
    let mut pm = ctx.pm.lock().await;
    span.record("process.restarted", pm.alive());
    pm.start(req.k, &ctx.exe_path, &ctx.cwd).await.map_err(|e| {
        span.record("init.outcome", "spawn_failed");
        tracing::error!(error.kind = "spawn", error.message = %e);
        AppError(StatusCode::INTERNAL_SERVER_ERROR, e)
    })?;
    span.record("init.outcome", "ok");
    Ok(Json(InitResponse {
        status: "ok".into(),
        k: req.k,
    }))
}

#[tracing::instrument(
    skip_all,
    fields(
        command.name = tracing::field::Empty,
        command.argv_len = req.args.len(),
        command.outcome = tracing::field::Empty,
        command.elapsed_ms = tracing::field::Empty,
    )
)]
async fn api_command(
    State(ctx): State<Ctx>,
    Json(req): Json<CommandRequest>,
) -> Result<Json<CommandResponse>, AppError> {
    let span = tracing::Span::current();
    let cmd = req.command.trim().to_string();
    span.record("command.name", cmd.as_str());

    let start = std::time::Instant::now();
    let mut pm = ctx.pm.lock().await;
    if !pm.alive() {
        span.record("command.outcome", "process_dead");
        span.record("command.elapsed_ms", start.elapsed().as_millis() as u64);
        tracing::error!(error.kind = "process_dead", "child not running");
        return Err(AppError(
            StatusCode::BAD_REQUEST,
            "Process not running. Call POST /api/init first.".into(),
        ));
    }

    if cmd == "fin" {
        pm.stop().await;
        span.record("command.outcome", "fin");
        span.record("command.elapsed_ms", start.elapsed().as_millis() as u64);
        return Ok(Json(CommandResponse {
            output: "Process terminated.".into(),
            lines: vec![],
        }));
    }

    let output = pm.send_command(&cmd, &req.args).await.map_err(|e| {
        span.record("command.outcome", "send_failed");
        span.record("command.elapsed_ms", start.elapsed().as_millis() as u64);
        tracing::error!(error.kind = "send_failed", error.message = %e);
        AppError(StatusCode::BAD_REQUEST, e)
    })?;

    let lines: Vec<String> = output
        .lines()
        .filter(|l| !l.trim().is_empty())
        .map(String::from)
        .collect();
    span.record("command.outcome", "ok");
    span.record("command.elapsed_ms", start.elapsed().as_millis() as u64);
    Ok(Json(CommandResponse { output, lines }))
}

#[tracing::instrument(
    skip_all,
    fields(
        payload.species_parsed = req.species.len(),
        payload.bytes = tracing::field::Empty,
        parse.outcome = tracing::field::Empty,
    )
)]
async fn api_read_species(
    State(ctx): State<Ctx>,
    Json(req): Json<ReadSpeciesRequest>,
) -> Result<Json<CommandResponse>, AppError> {
    let span = tracing::Span::current();
    let mut pm = ctx.pm.lock().await;
    if !pm.alive() {
        span.record("parse.outcome", "process_dead");
        tracing::error!(error.kind = "process_dead", "child not running");
        return Err(AppError(
            StatusCode::BAD_REQUEST,
            "Process not running. Call POST /api/init first.".into(),
        ));
    }

    let n = req.species.len();
    let mut raw_input = format!("lee_cjt_especies\n{n}\n");
    for sp in &req.species {
        raw_input.push_str(&format!("{} {}\n", sp.id, sp.gen));
    }
    span.record("payload.bytes", raw_input.len());

    let output = pm.send_raw_and_collect(&raw_input).await;
    let lines: Vec<String> = output
        .lines()
        .filter(|l| !l.trim().is_empty())
        .map(String::from)
        .collect();
    span.record("parse.outcome", "ok");
    Ok(Json(CommandResponse { output, lines }))
}

#[tracing::instrument(
    skip_all,
    fields(
        process.alive = tracing::field::Empty,
        process.k = tracing::field::Empty,
    )
)]
async fn api_status(State(ctx): State<Ctx>) -> Json<StatusResponse> {
    let pm = ctx.pm.lock().await;
    let alive = pm.alive();
    let k = pm.k_value;
    let span = tracing::Span::current();
    span.record("process.alive", alive);
    if let Some(k_val) = k {
        span.record("process.k", k_val);
    }
    Json(StatusResponse { alive, k })
}

#[tokio::main]
async fn main() {
    let _otel = observability::init_otel("pracpro2");

    let port: u16 = std::env::var("PORT")
        .ok()
        .and_then(|v| v.parse().ok())
        .unwrap_or(8000);

    let exe_path = std::env::var("EXE_PATH")
        .map(PathBuf::from)
        .unwrap_or_else(|_| PathBuf::from("./program.exe"));

    let cwd = exe_path
        .parent()
        .unwrap_or_else(|| std::path::Path::new("."))
        .to_path_buf();

    let static_dir = std::env::var("STATIC_DIR")
        .map(PathBuf::from)
        .unwrap_or_else(|_| PathBuf::from("./web/static"));

    let ctx = Ctx {
        pm: Arc::new(Mutex::new(ProcessManager::new())),
        exe_path,
        cwd,
    };

    let api = Router::new()
        .route("/api/init", post(api_init))
        .route("/api/command", post(api_command))
        .route("/api/read_species", post(api_read_species))
        .route("/api/status", get(api_status))
        .with_state(ctx);

    let app = api
        .fallback_service(
            ServeDir::new(&static_dir).fallback(tower_http::services::ServeFile::new(
                static_dir.join("index.html"),
            )),
        )
        .layer(axum::middleware::from_fn(session_id_middleware))
        .layer(CorsLayer::permissive());

    let listener = tokio::net::TcpListener::bind(format!("0.0.0.0:{port}"))
        .await
        .expect("Failed to bind");

    tracing::info!("listening on 0.0.0.0:{port}");
    axum::serve(listener, app).await.expect("server error");
}

#[cfg(test)]
mod tests {
    use super::*;
    use axum::body::Body;
    use axum::http::{self, Request};
    use http_body_util::BodyExt;
    use tower::ServiceExt;

    fn test_ctx() -> Ctx {
        Ctx {
            pm: Arc::new(Mutex::new(ProcessManager::new())),
            exe_path: PathBuf::from("/nonexistent/program.exe"),
            cwd: PathBuf::from("/tmp"),
        }
    }

    fn test_router() -> Router {
        Router::new()
            .route("/api/init", post(api_init))
            .route("/api/command", post(api_command))
            .route("/api/read_species", post(api_read_species))
            .route("/api/status", get(api_status))
            .with_state(test_ctx())
    }

    async fn body_string(body: Body) -> String {
        let bytes = body.collect().await.unwrap().to_bytes();
        String::from_utf8(bytes.to_vec()).unwrap()
    }

    #[tokio::test]
    async fn test_status_no_process() {
        let app = test_router();
        let resp = app
            .oneshot(
                Request::builder()
                    .uri("/api/status")
                    .body(Body::empty())
                    .unwrap(),
            )
            .await
            .unwrap();

        assert_eq!(resp.status(), StatusCode::OK);
        let body = body_string(resp.into_body()).await;
        let v: serde_json::Value = serde_json::from_str(&body).unwrap();
        assert_eq!(v["alive"], false);
        assert_eq!(v["k"], serde_json::Value::Null);
    }

    #[tokio::test]
    async fn test_init_invalid_k() {
        let app = test_router();
        let resp = app
            .oneshot(
                Request::builder()
                    .method(http::Method::POST)
                    .uri("/api/init")
                    .header("content-type", "application/json")
                    .body(Body::from(r#"{"k": 0}"#))
                    .unwrap(),
            )
            .await
            .unwrap();

        assert_eq!(resp.status(), StatusCode::BAD_REQUEST);
    }

    #[tokio::test]
    async fn test_init_negative_k() {
        let app = test_router();
        let resp = app
            .oneshot(
                Request::builder()
                    .method(http::Method::POST)
                    .uri("/api/init")
                    .header("content-type", "application/json")
                    .body(Body::from(r#"{"k": -5}"#))
                    .unwrap(),
            )
            .await
            .unwrap();

        assert_eq!(resp.status(), StatusCode::BAD_REQUEST);
    }

    #[tokio::test]
    async fn test_command_without_init() {
        let app = test_router();
        let resp = app
            .oneshot(
                Request::builder()
                    .method(http::Method::POST)
                    .uri("/api/command")
                    .header("content-type", "application/json")
                    .body(Body::from(r#"{"command": "foo"}"#))
                    .unwrap(),
            )
            .await
            .unwrap();

        assert_eq!(resp.status(), StatusCode::BAD_REQUEST);
        let body = body_string(resp.into_body()).await;
        assert!(body.contains("not running"));
    }

    #[tokio::test]
    async fn test_read_species_without_init() {
        let app = test_router();
        let resp = app
            .oneshot(
                Request::builder()
                    .method(http::Method::POST)
                    .uri("/api/read_species")
                    .header("content-type", "application/json")
                    .body(Body::from(r#"{"species": []}"#))
                    .unwrap(),
            )
            .await
            .unwrap();

        assert_eq!(resp.status(), StatusCode::BAD_REQUEST);
    }

    #[test]
    fn test_status_response_serialization() {
        let resp = StatusResponse {
            alive: true,
            k: Some(5),
        };
        let json = serde_json::to_value(&resp).unwrap();
        assert_eq!(json["alive"], true);
        assert_eq!(json["k"], 5);
    }

    #[test]
    fn test_status_response_null_k() {
        let resp = StatusResponse {
            alive: false,
            k: None,
        };
        let json = serde_json::to_value(&resp).unwrap();
        assert_eq!(json["alive"], false);
        assert!(json["k"].is_null());
    }

    #[test]
    fn test_init_response_serialization() {
        let resp = InitResponse {
            status: "ok".into(),
            k: 3,
        };
        let json = serde_json::to_value(&resp).unwrap();
        assert_eq!(json["status"], "ok");
        assert_eq!(json["k"], 3);
    }

    #[test]
    fn test_command_response_serialization() {
        let resp = CommandResponse {
            output: "hello\nworld".into(),
            lines: vec!["hello".into(), "world".into()],
        };
        let json = serde_json::to_value(&resp).unwrap();
        assert_eq!(json["output"], "hello\nworld");
        assert_eq!(json["lines"].as_array().unwrap().len(), 2);
    }

    #[test]
    fn test_process_manager_initial_state() {
        let pm = ProcessManager::new();
        assert!(!pm.alive());
        assert_eq!(pm.k_value, None);
    }

    #[test]
    fn test_menu_lines_contains_expected() {
        let ml = menu_lines();
        assert!(ml.contains("fin"));
        assert!(ml.contains("crea_especie"));
        assert!(ml.contains("tabla_distancias"));
        assert!(!ml.contains("nonexistent"));
    }

    #[test]
    fn test_strip_echo() {
        assert_eq!(strip_echo("  \nhello\nworld\n  \n"), "hello\nworld");
        assert_eq!(strip_echo(""), "");
        assert_eq!(strip_echo("   \n\n   "), "");
        assert_eq!(strip_echo("single"), "single");
        assert_eq!(strip_echo("# comment\ndata"), "data");
    }
}
