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

clean:
	rm *.o
	rm -r html
	rm -r latex
	rm *.exe
	rm practica.tar
