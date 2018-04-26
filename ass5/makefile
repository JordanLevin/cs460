FLAGS = -std=c++14 -lglut -lGLU -lGL -lm -O3

main: main.o 
	g++ main.o -o main $(FLAGS)

main.o: main.cpp utils.h
	g++ main.cpp -c $(FLAGS)

clean:
	rm -f main *.o
