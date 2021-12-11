#!/bin/bash

make clean
wait
make
wait
sudo rmmod hashbrown
sudo insmod hashbrown.ko
wait
sudo mknod /dev/hashbrown c 111 0
sudo chmod a+rw /dev/hashbrown