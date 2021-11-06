#!/bin/bash
make -s Help
make -s main
./Help
./main $1
make -s clean
