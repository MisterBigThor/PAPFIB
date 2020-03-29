#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */


int main(int argc, char *argv[]) {
	#pragma omp parallel num_threads(3)
	#pragma omp single
	{
		printf("Only one thread here, I'm %u!\n", omp_get_thread_num());
	}
	#pragma omp single
	{
		printf("Only one thread here, I'm %u!\n.", omp_get_thread_num());
	}
}
