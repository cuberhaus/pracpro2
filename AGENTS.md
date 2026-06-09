# pracpro2

FIB-UPC Pro2 course project (Spring 2020): species clustering by gene-sequence similarity using the WPGMA hierarchical algorithm. Original C++17 deliverable lives at the repo root; a Rust/Axum backend and Vue 3 + D3.js frontend in [web/](web/) modernize it into an interactive web visualization of the dendrogram.

## Architecture
- **C++ engine (frozen coursework)** — root `.cc`/`.hh` files: `program.cc` (CLI driver), `especie` / `cjt_especies` (species + distance table), `cluster` / `cjt_clusters` (WPGMA), `BinTree.hh` (FIB-provided tree).
- **Rust backend** — [web/backend/](web/backend/) Axum server that spawns the compiled `program.exe` and pipes commands over stdin/stdout.
- **Frontend** — [web/static/](web/static/) Vue 3 (CDN) single-page app with a D3.js dendrogram.

## Build and Test
- C++: `make` builds `program.exe`; `make run` runs the CLI; `make clean` clears artifacts.
- Web (dev): `make dev` compiles C++ then starts the Rust server on `:8000` (`EXE_PATH=../../program.exe STATIC_DIR=../static`).
- Web (release): `make build-backend` (cargo release build under `web/backend/`).
- Docker: `make docker-build` then `make docker-run` (port 8000).
- Tests: stdin/stdout fixtures in [jocs_de_prova/](jocs_de_prova/) — run `./program.exe < jocs_de_prova/entrada.txt > out.txt` and `diff` against `jocs_de_prova/correct.txt`.
- Docs: `make html` runs Doxygen into `html/` and `latex/`.

## Pitfalls
- The C++ deliverable is the graded submission — **treat it as frozen**. Do not rename files, change the CLI command vocabulary (`crea_especie`, `ejecuta_paso_wpgma`, etc.), or alter output formatting; the `jocs_de_prova` diffs depend on exact whitespace.
- Makefile compiles with `-Werror -Wextra -D_GLIBCXX_DEBUG -std=c++11` despite the README saying C++17 — any edit must stay warning-clean under those flags.
- The Rust backend has no real logic of its own; it shells out to `program.exe`. The binary must exist at `EXE_PATH` before `cargo run`, and parsing assumes the exact C++ stdout format.
- All user-facing strings (commands, errors, README) are in Spanish/Catalan — preserve language when touching them.

See [README.md](README.md).
