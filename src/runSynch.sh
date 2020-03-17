#! /bin/bash

cd ../test
echo "min omp version:"
echo "===================="
OMP_NUM_THREADS=10./tsynch-omp
echo "===================="
echo "===================="
OMP_NUM_THREADS=10 ./tsynch-gomp
echo "===================="

echo "endscript"


