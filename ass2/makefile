main: main.cpp main.h input.h draw.h
	g++ -std=c++14 -o main main.cpp -lglut -lGLU -lGL -lm -O3

test: main.cpp main.h input.h draw.h
	g++ -std=c++14 -g -o main main.cpp -lglut -lGLU -lGL -lm

clean:
	rm main
