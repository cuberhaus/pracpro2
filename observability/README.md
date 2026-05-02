# Observability — Honeycomb PoC for `pracpro2`

This branch (`obs-experiment-honeycomb`) replaces the prior Sentry
instrumentation with a `tracing` + `tracing-opentelemetry` + OTLP gRPC
pipeline pointed at Honeycomb's free tier. It is the second PoC in the
demo-repo observability series, paired against the LGTM PoC on
`Practica_de_Planificacion`.

## Why Honeycomb specifically

Honeycomb's pitch is "wide events instead of metrics + logs + traces":
you record one fat span per request with as many fields as you can think
of, then explore via the query builder and BubbleUp (anomaly explanation)
afterwards. Rust's `tracing` crate is the natural fit because the
`#[instrument(fields(...))]` syntax already encourages stuffing every
useful value onto the span.

This PoC deliberately picks a different shape from the LGTM stack:
- LGTM = self-hosted, multi-container, columnar metrics + structured logs + traces
- Honeycomb = SaaS, single signal type (spans), wide events, BubbleUp

The eventual `obs-experiment-notes.md` (Phase E) compares them
side-by-side on the same two scripted scenarios (latency regression,
error spike).

## Architecture

```text
                                   X-Session-Id propagated
                                      from parent portfolio
                                              |
+------------------+    OTLP gRPC :443       +-----------+
| pracpro2 axum    |  ----TLS---->           | Honeycomb |
| (instrumented)   |  x-honeycomb-team       | api.    | (SaaS)
| tracing crate    |                         | honey-    |
| -> OTel SDK      |                         | comb.io   |
+------------------+                         +-----------+
       |
       | child stdin/stdout
       v
+------------------+
| C++ program.exe  |  spans wrapped at the IPC boundary
+------------------+  (send_command span captures elapsed_ms,
                       output_bytes, lines, etc.)
```

Zero local containers. The entire PoC is account + env vars + Rust code.

## Quickstart

1. **Sign up** at <https://ui.honeycomb.io/signup> (free tier — 20M events/month, 60-day retention).
2. **Create an environment** named `local-dev` and an **ingest key** scoped to it. Copy the key.
3. **Configure** the local env file:
   ```bash
   cp observability/.env.honeycomb.example observability/.env.honeycomb
   $EDITOR observability/.env.honeycomb     # paste the ingest key
   ```
4. **Build the C++ program** (only needed once, mirrors the existing dev flow):
   ```bash
   make program.exe
   ```
5. **Run with telemetry on**:
   ```bash
   set -a; source observability/.env.honeycomb; set +a
   cd web/backend && cargo run
   # browser tab → http://localhost:8000 → click around the WPGMA UI
   ```
6. **First trace check**: <https://ui.honeycomb.io/your-team/environments/local-dev/datasets/pracpro2>
   should show traces within ~30 s of the first request. Click any trace
   to see the `http_request` → `api_command` → `send_command` waterfall
   with the wide-event fields populated.

## What gets instrumented

| Span                  | Span name        | Key wide-event fields |
|-----------------------|------------------|------------------------|
| Request entry         | `http_request`   | `session.id`, `http.method`, `http.target` |
| Init handler          | `api_init`       | `request.k`, `process.restarted`, `init.outcome` |
| Command handler       | `api_command`    | `command.name`, `command.argv_len`, `command.outcome`, `command.elapsed_ms` |
| Read-species handler  | `api_read_species` | `payload.species_parsed`, `payload.bytes`, `parse.outcome` |
| Status handler        | `api_status`     | `process.alive`, `process.k` |
| Child-process boundary| `send_command`   | `child.command`, `child.argv_len`, `child.input_bytes`, `child.output_bytes`, `child.lines`, `child.elapsed_ms` |
| Process spawn         | `start`          | `child.k`, `child.pid` |

Errors come through as `error.kind=...` events on the active span,
which Honeycomb groups automatically.

## Files in this directory

- `.env.honeycomb.example` — the env-var template, safe to commit.
- `.env.honeycomb` — your local file with the real key, gitignored.
- `README.md` — this file.
- `QUERIES.md` — three Honeycomb queries / boards worth bookmarking.
- `SCENARIOS.md` — two scripted experiments mirroring the LGTM PoC for
  side-by-side comparison.
- `obs-experiment-notes.md` (Phase E) — the comparison artefact you fill
  in as you actually use Honeycomb.

## What's removed vs. the prior `master` branch

- `web/backend/Cargo.toml` — `sentry`, `sentry-tower`, `sentry-tracing` deps gone.
- `web/backend/src/main.rs` — `_init_sentry()`, `_session_id_middleware`,
  `NewSentryLayer` layer, all Sentry imports gone.
- `Dockerfile` — Sentry-version-pinning comment gone (Rust 1.88 pin kept
  for reproducibility, no longer for SDK compatibility).
- `Cargo.lock` — Sentry transitive deps gone; refreshed via `cargo update`.

`rg -n 'sentry|Sentry|SENTRY' . --glob '!target'` returns zero matches.

## Coexistence with the portfolio

The portfolio orchestrator (`PersonalPortfolio/scripts/dev-all-demos.sh`)
sets `SENTRY_DSN`/`SENTRY_ENVIRONMENT`/`SENTRY_RELEASE` for the pracpro2
container as part of the multi-stack development workflow. With Sentry
deleted on this branch, those env vars are simply ignored — pracpro2 won't
emit Sentry events, and the orchestrator doesn't need to change.

OTLP env vars are additive: the orchestrator can set
`OTEL_EXPORTER_OTLP_ENDPOINT` etc. for the pracpro2 container without
touching any other backend.
