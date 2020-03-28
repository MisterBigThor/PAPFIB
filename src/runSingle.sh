#! /bin/bash

cd ../test

echo "===================="
make tsingle1-omp
OMP_NUM_THREADS=3 ./tsingle1-omp
echo "********************"

echo "===================="
make tsingle1-gomp
OMP_NUM_THREADS=3 ./tsingle1-gomp
echo "********************"


