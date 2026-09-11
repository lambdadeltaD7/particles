
all: main


main: main.cpp vec.cpp world.cpp world.h vec.h
	g++ *.cpp -O3 -lSDL3 -o main

debug: main.cpp vec.cpp world.cpp world.h vec.h
	g++ -g *.cpp -lSDL3 -o main

