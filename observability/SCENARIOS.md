# Scripted scenarios — like-for-like with the LGTM PoC

These two experiments produce comparable observations across the LGTM
and Honeycomb PoCs. Run each, capture screenshots / time-to-detection,
write them up in `obs-experiment-notes.md`.

The expectation is **not** that one stack "wins" — it's that the
trade-offs become concrete instead of theoretical.

## Scenario A — latency regression

**Setup**: a hidden `tokio::time::sleep(Duration::from_millis(500))`
inside `api_command` for the `tabla_distancias` command only. The
regression is invisible in p50, mildly visible in p95, glaring in p99.

Add this temporary block at the top of the `tabla_distancias` branch
inside `send_command` (revert when done):

```rust
if command == "tabla_distancias" {
    tokio::time::sleep(std::time::Duration::from_millis(500)).await;
}
```

**Drive load** (any of these works; pick one and stick with it across
both PoCs so the comparison is fair):

```bash
# Simple curl loop — generates ~10 req/s
while true; do
  curl -s -X POST http://localhost:8000/api/command \
       -H 'content-type: application/json' \
       -H 'X-Session-Id: scenario-a-baseline' \
       -d '{"command":"tabla_distancias"}' > /dev/null
  sleep 0.1
done
```

**Comparison axes**:

| Question                                   | Honeycomb answer | LGTM answer |
|--------------------------------------------|-------------------|--------------|
| Time from regression to noticing it?       | (Query #1 board: p99 spike on `tabla_distancias`) | (Mimir alert on `histogram_quantile(0.99, ...)`) |
| Clicks from "it's slow" to "it's this command"? | 1 (group by `command.name` in the query)   | 2 (alert → dashboard → drilldown) |
| Did you spot the 500 ms band as a separate cluster? | (heatmap on `child.elapsed_ms`) | (Tempo trace search by latency) |
| Cost / friction to ask follow-up questions? | (free-form query rebuilds in seconds) | (PromQL recording rule + dashboard reload) |

## Scenario B — error spike

**Setup**: throw on every 10th request to `api_command`. Add a counter
in `api_command` and `Err` once when it hits 10.

Add this temporary block inside `api_command` (revert when done):

```rust
use std::sync::atomic::{AtomicU32, Ordering};
static REQ_COUNTER: AtomicU32 = AtomicU32::new(0);
let n = REQ_COUNTER.fetch_add(1, Ordering::Relaxed);
if n % 10 == 9 {
    span.record("command.outcome", "synthetic_error");
    tracing::error!(error.kind = "synthetic_error", "scenario B injection");
    return Err(AppError(StatusCode::INTERNAL_SERVER_ERROR, "scenario B".into()));
}
```

**Drive load**: same curl loop as Scenario A, plus run for 10 minutes.

**Comparison axes**:

| Question                                                    | Honeycomb answer | LGTM answer |
|-------------------------------------------------------------|-------------------|--------------|
| Time from spike to alert firing?                            | Trigger #4 (5 min eval) | Mimir alert rule (similar) |
| Are errors grouped automatically?                           | yes (`error.kind`) | no (manual `group by`) |
| Can you see what's *different* about failing requests vs. healthy ones? | BubbleUp (1 click)| manually in Loki / Tempo (3 clicks) |
| Sample replay / stack trace?                                | span events with stacktrace | requires Sentry (separate stack)|
| Effort to keep this experiment running for a week?          | none (SaaS)       | non-zero (3 GB MinIO budget on the LGTM stack)|

## Reverting the scenarios

Both scenarios edit `web/backend/src/main.rs` temporarily. Use:

```bash
git diff web/backend/src/main.rs           # see what's still injected
git checkout web/backend/src/main.rs       # nuke the scenario edits
```

…to restore the clean instrumented state once observations are captured.

## Optional: make-targets

Once the scenarios stabilise, lift them to the Makefile:

```make
obs-scenario-a:
	bash observability/scenarios/run-latency.sh
obs-scenario-b:
	bash observability/scenarios/run-errors.sh
```

Out of scope for this initial branch but a natural follow-up.
