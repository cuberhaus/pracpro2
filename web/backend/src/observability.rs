//! OpenTelemetry → OTLP wiring for the pracpro2 backend.
//!
//! Replaces the prior Sentry instrumentation (deleted on this branch). All
//! Honeycomb-specific config lives in env vars so the same code points at
//! any OTLP-compatible backend (OpenObserve, SigNoz, a local Collector).
//!
//! Required env vars (loaded by the caller, e.g. via shell `set -a; source
//! .env.honeycomb` or systemd EnvironmentFile=):
//!
//!   OTEL_SERVICE_NAME=pracpro2
//!   OTEL_EXPORTER_OTLP_ENDPOINT=https://api.honeycomb.io:443
//!   OTEL_EXPORTER_OTLP_HEADERS=x-honeycomb-team=<API_KEY>
//!   OTEL_RESOURCE_ATTRIBUTES=service.namespace=cuberhaus,deployment.environment=local-dev
//!
//! When `OTEL_EXPORTER_OTLP_ENDPOINT` is unset, `init_otel` skips the OTel
//! pipeline entirely and falls back to local JSON logs only — useful for
//! `cargo test` and for demoing the binary without a Honeycomb account.

use opentelemetry::global;
use opentelemetry::trace::TracerProvider as _;
use opentelemetry::KeyValue;
use opentelemetry_otlp::SpanExporter;
use opentelemetry_sdk::trace::SdkTracerProvider;
use opentelemetry_sdk::Resource;
use opentelemetry_semantic_conventions::attribute::SERVICE_NAME;
use tracing_subscriber::layer::SubscriberExt;
use tracing_subscriber::util::SubscriberInitExt;
use tracing_subscriber::{fmt, EnvFilter};

/// Returned from `init_otel`; flushes the provider on Drop so that any
/// in-flight batch isn't lost when the server is interrupted.
pub struct OtelGuard {
    provider: SdkTracerProvider,
}

impl Drop for OtelGuard {
    fn drop(&mut self) {
        if let Err(e) = self.provider.shutdown() {
            eprintln!("otel: provider shutdown failed: {e:?}");
        }
    }
}

/// Initialise tracing-subscriber with three layers (env-filter, JSON
/// stdout, OTel) when an OTLP endpoint is configured, or two layers
/// (env-filter, JSON stdout) when it isn't.
///
/// Returns `Some(OtelGuard)` only when the OTel pipeline was actually
/// stood up; the caller binds it to a local in `main()` to keep it alive.
pub fn init_otel(default_service: &str) -> Option<OtelGuard> {
    let endpoint = std::env::var("OTEL_EXPORTER_OTLP_ENDPOINT")
        .ok()
        .filter(|s| !s.is_empty());

    let mk_filter =
        || EnvFilter::try_from_default_env().unwrap_or_else(|_| EnvFilter::new("info"));

    if endpoint.is_none() {
        let _ = tracing_subscriber::registry()
            .with(mk_filter())
            .with(fmt::layer().json())
            .try_init();
        eprintln!(
            "otel: OTEL_EXPORTER_OTLP_ENDPOINT unset; running with stdout JSON logs only"
        );
        return None;
    }

    let service_name =
        std::env::var("OTEL_SERVICE_NAME").unwrap_or_else(|_| default_service.to_string());

    let exporter = SpanExporter::builder()
        .with_tonic()
        .build()
        .expect("failed to build OTLP gRPC exporter");

    let provider = SdkTracerProvider::builder()
        .with_batch_exporter(exporter)
        .with_resource(
            Resource::builder()
                .with_attribute(KeyValue::new(SERVICE_NAME, service_name.clone()))
                .build(),
        )
        .build();

    global::set_tracer_provider(provider.clone());

    let tracer = provider.tracer(service_name);

    let _ = tracing_subscriber::registry()
        .with(mk_filter())
        .with(fmt::layer().json())
        .with(tracing_opentelemetry::layer().with_tracer(tracer))
        .try_init();

    Some(OtelGuard { provider })
}
