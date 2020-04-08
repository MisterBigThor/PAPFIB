#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

long result=0;

int matrix[10][10];

void foo() {
	#pragma omp parallel
    	{
    		#pragma omp for schedule(dynamic,4)
	    	for (long i = 0; i < 10; i++){
			for(long j = 0; j <10; j++)matrix[i][j] = i+j;
		}

    		#pragma omp single
    		printf("Inside foo ... result = %ld\n", result);

		#pragma omp for schedule(dynamic,4)
	    	for (long i = 0; i < 10; i++){
			for(long j = 0; j < 10; j++){
				printf("[%li][%li]: %i ", i, j, matrix[i][j]+1);
			}
			printf("\n");
		}
	}
}

int main(int argc, char *argv[]) {
    foo();
}
