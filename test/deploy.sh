#!/bin/bash

diskutil mount /dev/$2
./build.sh
cp ./build/$1.uf2 /Volumes/PICO/deploy.uf2
diskutil umount /Volumes/PICO/
