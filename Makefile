
all: main


main: main.cpp vec.cpp world.cpp
	g++ *.cpp -lSDL3 -o main
