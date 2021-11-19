#!/bin/bash
sudo apt-get install libsfml-dev 
g++ -w -o Help Help.cpp -lsfml-graphics -lsfml-window -lsfml-system
g++ -w -O3 -ffast-math -Wall -o main main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread 
 
./Help
if [ $# -eq 0 ]
  then
    ./main exp-res 
   else
    ./main $1 
fi

rm -f *.o *.png main Help dump
