FLAGS = -std=c++14 -lglut -lGLU -lGL -lm -O3

main: main.cpp triangle.o flower.o basics.o
	g++ $(FLAGS) main.cpp triangle.o flower.o basics.o -o main

triangle.o: triangle.cpp triangle.h basics.h
	g++ $(FLAGS) triangle.cpp -c 

flower.o: flower.cpp flower.h triangle.h basics.h
	g++ $(FLAGS) flower.cpp -c

basics.o: basics.cpp basics.h
	g++ $(FLAGS) basics.cpp -c

clean:
	rm -f main *.o
