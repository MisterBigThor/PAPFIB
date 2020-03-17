#! /bin/bash

cd ../test
make tworkshare-omp
make tworkshare-gomp

echo "min omp version"
OMP_NUM_THREADS=10 ./tworkshare-omp
echo "===================="
OMP_NUM_THREADS=10 ./tworkshare-gomp
echo "end"


