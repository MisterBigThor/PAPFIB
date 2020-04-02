
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */


int main(int argc, char *argv[]) {
	printf("Test 1: Parallel with a single");
	int i = 4;
	#pragma omp parallel
	{
		#pragma omp single
		{
			printf("Only one thread here, I'm %u!\n", omp_get_thread_num());
			i += 4;
		}
		//printf("Hi!, I'm thread %u\n", omp_get_thread_num());
	}
	printf("Expected 8, result: %u\n", i);
	printf("****====****====****====****\n");
	printf("Test 2: Parallel single with a no wait\n");
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			sleep(3);
			printf("Last message: Only one thread here!\n");
//			printf("Last message: Only one sleep here here, I'm %u!\n", omp_get_thread_num());

		}
		#pragma omp single nowait
		{
			printf("Second single. Only one thread here\n");
//			printf("Secons single: Only one thread here, I'm %u!\n", omp_get_thread_num());
		}
		//printf("Hi!, I'm thread %u\n", omp_get_thread_num());
	}
	printf("****====****====****====****\n");
}
