#!/bin/bash

mkdir ./pi
sudo mount /dev/$1 pi
./build.sh
cp ./build/main.uf2 ./pi/deploy.uf2
sudo umount ./pi
sudo rm -R ./pi
