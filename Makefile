OPCIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11

practica.tar: program.exe 
	tar -cvf practica.tar program.cc especie.hh cjt_especies.hh especie.cc cjt_especies.cc Makefile
program.exe: program.o especie.o cjt_especies.o
	g++ -o program.exe program.o especie.o cjt_especies.o
program.o: program.cc especie.hh cjt_especies.hh 
	g++ -c program.cc especie.hh cjt_especies.hh $(OPCIONS)
especie.o: especie.cc especie.hh
	g++ -c especie.cc especie.hh $(OPCIONS)
cjt_especies.o: cjt_especies.cc especie.hh cjt_especies.hh 
	g++ -c cjt_especies.cc especie.hh cjt_especies.hh $(OPCIONS)

clean:
	rm *.o
	rm *.exe
	rm practica.tar
