a.out: main.o
	g++ main.cpp -o a.out

run: a.out
	./a.out