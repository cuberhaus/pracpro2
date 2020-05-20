OPCIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11

practica.tar: program.exe html
	tar -cvf practica.tar program.cc especie.hh cjt_especies.hh especie.cc cjt_especies.cc Makefile cjt_clusters.cc cjt_clusters.hh cluster.cc cluster.hh html Doxyfile
html: Doxyfile
	doxygen
program.exe: program.o especie.o cjt_especies.o cjt_clusters.o cluster.o
	g++ -o program.exe program.o especie.o cjt_especies.o cjt_clusters.o cluster.o
program.o: program.cc especie.hh cjt_especies.hh
	g++ -c program.cc especie.hh cjt_especies.hh $(OPCIONS)
especie.o: especie.cc especie.hh
	g++ -c especie.cc especie.hh $(OPCIONS)
cjt_especies.o: cjt_especies.cc especie.hh cjt_especies.hh
	g++ -c cjt_especies.cc especie.hh cjt_especies.hh $(OPCIONS)
cjt_clusters.o: cjt_clusters.cc cjt_clusters.hh cluster.hh BinTree.hh
	g++ -c cjt_clusters.cc cluster.hh cjt_clusters.hh BinTree.hh $(OPCIONS)
cluster.o: cluster.cc cluster.hh BinTree.hh
	g++ -c cluster.cc cluster.hh BinTree.hh $(OPCIONS)
clean:
	rm *.o
	rm -r html
	rm -r latex
	rm *.exe
	rm practica.tar
