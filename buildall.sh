#!/usr/bin/env bash
./clean.sh
cd ./lib/glew/
make
cd ../../
cmake -DSIMONLY=OFF .
make

