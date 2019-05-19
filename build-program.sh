#!/bin/bash

./build.sh
./cvm > generated.asm
gcc -c stub.c -g
nasm -fmacho64 generated.asm -g
gcc -g stub.o generated.o
