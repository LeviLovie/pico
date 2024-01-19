#!/bin/bash

diskutil mount /dev/$2
./build.sh
sudo cp ./build/$1.uf2 /Volumes/RPI-RP2/app.uf2
# diskutil umount /Volumes/RPI-RP2/
