# Stage 1: Compile C++ program
FROM gcc:13 AS cpp-build
WORKDIR /src
COPY *.cc *.hh Makefile ./
RUN make program.exe

# Stage 2: Build Rust backend
FROM rust:1.85-slim AS rust-build
WORKDIR /src
RUN apt-get update && apt-get install -y pkg-config && rm -rf /var/lib/apt/lists/*
COPY web/backend/Cargo.toml web/backend/Cargo.lock* ./
RUN mkdir src && echo 'fn main(){}' > src/main.rs && cargo build --release && rm -rf src
COPY web/backend/src/ ./src/
RUN touch src/main.rs && cargo build --release

# Stage 3: Minimal runtime
FROM debian:bookworm-slim
WORKDIR /app
COPY --from=cpp-build /usr/local/lib64/libstdc++.so.6* /usr/local/lib64/
ENV LD_LIBRARY_PATH=/usr/local/lib64
COPY --from=cpp-build /src/program.exe ./
COPY --from=rust-build /src/target/release/pracpro2-backend ./server
COPY web/static/ ./static/

ENV PORT=8000
ENV EXE_PATH=/app/program.exe
ENV STATIC_DIR=/app/static
EXPOSE 8000

CMD ["./server"]
