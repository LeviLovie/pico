#!/bin/bash

mkdir ./pi
sudo mount /dev/$2 pi
./build.sh
cp ./build/$1.uf2 ./pi/deploy.uf2
sudo umount ./pi
sudo rm -R ./pi
