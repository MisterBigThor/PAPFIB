#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

#define N 100

int main(int argc, char *argv[]) {
	#pragma omp parallel
	for(int i = 0; i < N; ++i){
		#pragma omp single nowait
		printf("%i Only one thread here, I'm %u!\n", i,omp_get_thread_num());
	}
	printf("end\n");
}
