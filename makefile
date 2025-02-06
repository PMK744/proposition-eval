a.out: p01.o
	g++ p01.cpp -o a.out

run: a.out
	./a.out