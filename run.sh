#!/bin/sh

echo "single"
g++ -O3 -DSINGLE ./shared_mem.cpp -lrt
./a.out

echo "copy"
g++ -O3 -DCOPY ./shared_mem.cpp -lrt
./a.out

echo "shm"
g++ -O3 -DSHM ./shared_mem.cpp -rlt
./a.out

echo "mmap"
g++ -O3 -DMMAP ./shared_mem.cpp -lrt
./a.out

