
all: main


main: main.cpp vec.cpp world.cpp world.h vec.h
	g++ *.cpp -O3 -lSDL3 -lSDL3_ttf -o main

debug: main.cpp vec.cpp world.cpp world.h vec.h
	g++ -g *.cpp -lSDL3 -lSDL3_ttf -o main

