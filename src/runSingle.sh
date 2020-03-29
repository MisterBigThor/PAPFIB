#! /bin/bash

cd ../test

echo "======SINGLE1-omp==========="
make tsingle1-omp
OMP_NUM_THREADS=4 ./tsingle1-omp
echo "********************"

echo "======SINGLE1-gomp=========="
make tsingle1-gomp
OMP_NUM_THREADS=4 ./tsingle1-gomp
echo "********************"

echo "======SINGLE2-omp==========="
make tsingle2-omp
OMP_NUM_THREADS=4 ./tsingle2-omp
echo "********************"

echo "======SINGLE2-gomp=========="
make tsingle2-gomp
OMP_NUM_THREADS=4 ./tsingle2-gomp
echo "********************"


