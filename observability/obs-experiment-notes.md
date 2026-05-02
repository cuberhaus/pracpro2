# Honeycomb PoC — observation log

Fill this in as you actually use the stack. Once the LGTM PoC has its
own version with the same headings, the side-by-side becomes the
deliverable for the broader observability-experiment write-up.

Headings deliberately mirror the LGTM-PoC notes file so a `diff` is the
fastest comparison medium.

---

## First impressions

_Notes from the first 30 minutes of clicking around Honeycomb's UI after
the first traces land. What stood out, what was confusing, what was
surprisingly fast or slow._

- Time from `cargo run` to first trace in UI: TBD
- Time from first trace to first useful query: TBD
- Friction points: TBD

## Latency-regression UX (Scenario A)

_See [SCENARIOS.md](SCENARIOS.md#scenario-a--latency-regression). Capture
time-to-detection, number of clicks from "something's slow" to "this
specific command is slow", and any moment where Honeycomb's UI made the
diagnosis faster or slower than expected._

- Time from regression to noticing: TBD
- Clicks from notice → "it's `tabla_distancias`": TBD
- BubbleUp output worth screenshotting? Y/N + paste: TBD

## Error-spike UX (Scenario B)

_See [SCENARIOS.md](SCENARIOS.md#scenario-b--error-spike). Same axes:
time-to-alert, automatic vs. manual grouping, root-cause workflow._

- Time from injection to trigger firing: TBD
- Did `error.kind` grouping show up automatically? Y/N: TBD
- Was the failing-vs-healthy diff (BubbleUp on `error.kind`) useful? Y/N: TBD

## Cost & operational shape

_How much it costs to actually run this. Memory / disk / RPS limits,
free-tier ceilings, whether 60-day retention is enough for the kind of
investigation you actually want to do._

- Free-tier headroom for this PoC's traffic level: TBD
- Did you ever hit the 20M events / month ceiling: TBD
- 60-day retention enough? TBD
- Lock-in concerns (data export, query syntax): TBD

## Verdict

_The 1-paragraph "if you had to pick one for the portfolio's main
observability stack tomorrow" call. Reference Scenario A and B
observations._

TBD — write this last, after both PoCs have their own filled-in
versions.

---

## Cross-PoC comparison (filled in after the LGTM notes also exist)

| Axis                    | Honeycomb (this PoC) | LGTM (Practica_de_Planificacion) |
|-------------------------|----------------------|------------------------------------|
| Setup time              | TBD                  | TBD                                |
| First-meaningful-query  | TBD                  | TBD                                |
| Lat-regression detection| TBD                  | TBD                                |
| Error-spike detection   | TBD                  | TBD                                |
| Free-tier limits / cost | TBD                  | TBD                                |
| Lock-in risk            | TBD                  | TBD                                |
| Data export ease        | TBD                  | TBD                                |
| Best at                 | TBD                  | TBD                                |
| Worst at                | TBD                  | TBD                                |

The interesting cells are usually "best at" and "worst at" — they're
where the design choices actually matter for the kind of work the
portfolio is showing off.
