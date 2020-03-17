#! /bin/bash

cd ../test
make tworkshare-omp
make tworkshare-gomp
echo "===================="
echo "min omp version:"
OMP_NUM_THREADS=10 ./tworkshare-omp
echo "===================="
echo "gomp version:"
OMP_NUM_THREADS=10 ./tworkshare-gomp
echo "===================="
echo "end script"


