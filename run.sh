#!/bin/bash
make -s Help
make -s main
./Help
if [ $# -eq 0 ]
  then
    ./main exp-res
   else
    ./main $1
fi

make -s clean
