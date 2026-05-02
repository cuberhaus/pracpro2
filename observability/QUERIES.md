# Honeycomb queries to bookmark

Once telemetry is flowing, these are the three queries / boards that
quickly show whether the wide-event instrumentation pays off.

Save each one in Honeycomb's UI as a Board so they survive the trial
period.

## 1. P99 latency by command name (the RED-without-metrics view)

```
VISUALIZE  P99(command.elapsed_ms), COUNT
WHERE      name = "api_command"
GROUP BY   command.name
TIME       last 1 hour
```

Why this matters: the LGTM PoC computes this off pre-aggregated
`http_server_request_duration_seconds_bucket` metrics. Honeycomb computes
it on the raw events. If you change your mind tomorrow and want
"P95 latency by `command.name` for `session.id` IN (...)", the metrics
backend won't help you — Honeycomb will. That's the cardinality
trade-off.

## 2. Error rate by outcome

```
VISUALIZE  COUNT
WHERE      name = "api_command"
GROUP BY   command.outcome
TIME       last 1 hour
```

`command.outcome` is a span field with a small alphabet (`ok`, `fin`,
`process_dead`, `send_failed`). Stacked bars colour-coded by outcome let
you see at a glance whether the C++ child died, the args validation
caught something, or whether things are healthy. Two clicks deep into a
"process_dead" bar, BubbleUp tells you what *else* was different about
those failing requests (specific `command.name`? specific `session.id`?).

## 3. BubbleUp on slow child-process spans

```
VISUALIZE  HEATMAP(child.elapsed_ms)
WHERE      name = "send_command"
TIME       last 1 hour
```

Click the slow band on the heatmap → "BubbleUp" → Honeycomb compares the
distribution of every other field for slow vs. fast spans and highlights
the ones that differ most. Ideal for "what makes the slow species
operations slow" — a question that requires `child.command`,
`child.input_bytes`, `child.lines`, `process.k`, and `session.id` all
correlated, which is exactly what wide events were designed for.

## 4. Trigger: any `error.kind` span event

Configure as a Trigger (Honeycomb's alerting primitive) instead of a
saved query.

```
VISUALIZE  COUNT
WHERE      error.kind exists
GROUP BY   error.kind
THRESHOLD  COUNT > 5 per 5 min
```

Maps to "any handler emitted a `tracing::error!` event in the last 5
minutes". Notification destination: webhook to Slack / email / wherever.
Maps 1-1 to Mimir's `ALERT IF rate(errors_total[5m]) > X` in the LGTM
PoC; the comparison artefact in Phase E should note the syntactic
difference (Honeycomb queries vs. PromQL) but functional equivalence.
