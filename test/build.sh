#!/bin/bash

mkdir -p build
cd build
cmake -DPICO_BOARD=pico ..
make -j4
