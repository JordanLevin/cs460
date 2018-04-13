FLAGS = -std=c++14 -lglut -lGLU -lGL -lm -O3

main: main.o triangle.o flower.o basics.o
	g++ main.o triangle.o flower.o basics.o -o main $(FLAGS)

main.o: main.cpp
	g++ main.cpp -c $(FLAGS)

triangle.o: triangle.cpp triangle.h basics.h
	g++ triangle.cpp -c  $(FLAGS)

flower.o: flower.cpp flower.h triangle.h basics.h
	g++ flower.cpp -c $(FLAGS)

basics.o: basics.cpp basics.h
	g++ basics.cpp -c $(FLAGS)

clean:
	rm -f main *.o
