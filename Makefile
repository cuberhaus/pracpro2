OPCIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11

main.exe: cluster.o especie.o cjt_especies.o main.o
	g++ -o cluster.o especie.o cjt_especies.o $(OPCIONS)
cluster.o: cluster.hh cluster.cc BinTree.hh
	g++ -c cluster.hh cluster.cc BinTree.hh $(OPCIONS)
especie.o: especie.hh especie.cc
	g++ -c especie.hh especie.cc $(OPCIONS)
cjt_especies.o: cjt_especies.hh cjt_especies.cc cluster.hh cluster.cc especie.hh especie.cc
	g++ -c cjt_especies.hh cjt_especies.cc cluster.hh cluster.cc especie.hh especie.cc $(OPCIONS)
main.o: cjt_especies.hh cjt_especies.cc cluster.hh cluster.cc especie.hh especie.cc main.cc
	g++ -c cjt_especies.hh cjt_especies.cc cluster.hh cluster.cc especie.hh especie.cc main.cc $(OPCIONS)
clean:
	rm *.o
	rm *.exe
