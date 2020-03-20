#! /bin/bash

cd ../test
make tsingle1-omp
make tsingle1-gomp
echo "===================="
echo "min omp version:1"
./tsingle1-omp
echo "===================="
echo "gomp version:1"
./tsingle1-gomp
echo "===================="
echo "end script"


