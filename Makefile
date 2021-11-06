CC=g++
CFLAGS= -w -O3 -ffast-math -Wall
LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lpthread

main: main.o
	$(CC) $(CFLAGS) -o main main.cpp $(LIBS)
Help: Help.o
	$(CC) -o Help Help.cpp $(LIBS)    
clean:
	rm -f *.o *.png main Help

