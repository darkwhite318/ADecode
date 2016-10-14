
all: ADecode

ADecode: ADecode.o
	g++ -o ADecode ADecode.o -g -O3

ADecode.o: ADecode.cpp
	g++ -c ADecode.cpp -g -O3

clean:
	rm -f *.o
