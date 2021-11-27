#!/bin/sh

./clean.sh

render=1
if [ $# == 1 ]; then
    render=$1
fi

if [ $render == 0 ]; then
    sed -i 's/#define RENDER_ENABLED.*/#define RENDER_ENABLED 0/p' src/sim.h
    cmake -DRENDER=OFF .
fi

if [ $render == 1 ]; then
    sed -i 's/#define RENDER_ENABLED.*/#define RENDER_ENABLED 1/p' src/sim.h
    cmake -DRENDER=ON .
fi

make

