all: test

test: main.o PiecewiseConstantFunction.o
	g++ -g -o test main.o PiecewiseConstantFunction.o

main.o: main.cpp PiecewiseConstantFunction.h
	g++ -c main.cpp

PiecewiseConstantFunction.o: PiecewiseConstantFunction.cpp PiecewiseConstantFunction.h
	g++ -c PiecewiseConstantFunction.cpp

clean:
	rm -f test *.o
