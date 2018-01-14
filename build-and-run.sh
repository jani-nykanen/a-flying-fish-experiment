#!/bin/sh
make
if [ $? -eq 0 ]; then
    ./game $1 $2 $3
fi
