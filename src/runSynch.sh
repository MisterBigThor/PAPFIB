#! /bin/bash

cd ../test

echo "===================="
OMP_NUM_THREADS=10./tsynch-omp
echo "********************"

echo "===================="
OMP_NUM_THREADS=10 ./tsynch-gomp
echo "********************"


