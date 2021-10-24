all: main Help

CC=g++
CFLAGS=-O3 -ffast-math -w -Wall
LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lpthread

main: main.o
    $(CC) $(CFLAGS) -o main main.cpp $(LIBS) 

Help: Help.o
    g++ -o Help Help.cpp -lsfml-graphics -lsfml-window -lsfml-system
clean:
    rm -f *.o *.png main

