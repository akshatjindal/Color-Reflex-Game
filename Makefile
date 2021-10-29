all:
	g++ -std=c++11 -Wall -O0 -g -lsdl2 -lSDL2_image -lSDL2_ttf -Iinclude -Llib firstgo/main.cpp -o Play
