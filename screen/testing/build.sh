#!/bin/bash

mkdir build
cd build
cmake -DPICO_BOARD=pico_w ..
make -j4
