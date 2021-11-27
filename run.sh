#!/bin/sh
MYDIR=$(dirname $0)
LD_LIBRARY_PATH=$MYDIR/lib:$LD_LIBARY_PATH
export LD_LIBARY_PATH
#exec $MYDIR/Vortexy "$@"
./Vortexy -s sim.cfg

