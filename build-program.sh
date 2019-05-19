#!/bin/bash

./build.sh
./cvm > generated.asm
nasm -fmacho64 generated.asm -g
# nasm -felf64 generated.asm -g
gcc -g -fno-pie generated.o
