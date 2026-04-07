OPCIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11
CCCO = g++ -o
CCCC = g++ -c

practica.tar: program.exe html
	tar -cvf practica.tar program.cc especie.hh cjt_especies.hh especie.cc cjt_especies.cc Makefile cjt_clusters.cc cjt_clusters.hh cluster.cc cluster.hh html Doxyfile
html: Doxyfile
	doxygen
program.exe: program.o especie.o cjt_especies.o cjt_clusters.o cluster.o
	$(CCCO) program.exe program.o especie.o cjt_especies.o cjt_clusters.o cluster.o
program.o: program.cc especie.hh cjt_especies.hh
	$(CCCC) program.cc especie.hh cjt_especies.hh $(OPCIONS)
especie.o: especie.cc especie.hh
	$(CCCC) especie.cc especie.hh $(OPCIONS)
cjt_especies.o: cjt_especies.cc especie.hh cjt_especies.hh
	$(CCCC) cjt_especies.cc especie.hh cjt_especies.hh $(OPCIONS)
cjt_clusters.o: cjt_clusters.cc cjt_clusters.hh cluster.hh BinTree.hh
	$(CCCC) cjt_clusters.cc cluster.hh cjt_clusters.hh BinTree.hh $(OPCIONS)
cluster.o: cluster.cc cluster.hh BinTree.hh
	$(CCCC) cluster.cc cluster.hh BinTree.hh $(OPCIONS)

# THE CODE BELOW ALSO WORKS, KEEPING THE ABOVE JUST AS AN EXAMPLE	
#program.exe: program.o especie.o cjt_especies.o cjt_clusters.o cluster.o
#	$(CCCO) program.exe $^
#program.o: program.cc especie.hh cjt_especies.hh
#	$(CCCC) $^ $(OPCIONS)
#especie.o: especie.cc especie.hh
#	$(CCCC) $^ $(OPCIONS)
#cjt_especies.o: cjt_especies.cc especie.hh cjt_especies.hh
#	$(CCCC) $^ $(OPCIONS)
#cjt_clusters.o: cjt_clusters.cc cjt_clusters.hh cluster.hh BinTree.hh
#	$(CCCC) $^ $(OPCIONS)
#cluster.o: cluster.cc cluster.hh BinTree.hh
#	$(CCCC) $^ $(OPCIONS)

run:program.exe
	./program.exe

# ── Web frontend ──────────────────────────────────────

.PHONY: dev build-backend

build-backend: ## Build Rust backend (release)
	cd web/backend && cargo build --release

dev: program.exe ## Compile C++ and start Rust dev server on :8000
	cd web/backend && EXE_PATH=../../program.exe STATIC_DIR=../static cargo run

# ── Docker ────────────────────────────────────────────

.PHONY: docker-build docker-run

docker-build:
	docker build -t pracpro2 .

docker-run:
	docker run --rm -p 8000:8000 pracpro2
	@echo ""
	@echo "  PRO2 Phylogenetic Tree is running at:"
	@echo "    ➜  http://localhost:8000"
	@echo ""

# ── Cleanup ───────────────────────────────────────────

clean:
	rm -f *.o *.exe practica.tar
	rm -rf html latex

# ── Help ──────────────────────────────────────────────

.PHONY: help

help:
	@echo "Usage:"
	@echo "  make program.exe    Compile the C++ program"
	@echo "  make run            Compile and run the CLI program"
	@echo "  make dev            Compile + start Rust/Axum dev server on :8000"
	@echo "  make build-backend  Build Rust backend (release)"
	@echo "  make docker-build   Build Docker image"
	@echo "  make docker-run     Run Docker container on :8000"
	@echo "  make html           Generate Doxygen docs"
	@echo "  make practica.tar   Package submission tarball"
	@echo "  make clean          Remove build artifacts"
