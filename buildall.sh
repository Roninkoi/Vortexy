#!/usr/bin/env bash
cd ./lib/glew/
make
cd ../../
cmake -DSIMONLY=OFF .
make

