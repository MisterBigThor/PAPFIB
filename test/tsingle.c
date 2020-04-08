#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */
#include <unistd.h>
#

void doSingleStuff(){
	int d=rand()%4;
	printf("Thread %u, going to sleep %i\n", omp_get_thread_num(), d);
	sleep(d);
}

int main(int argc, char *argv[]) {
	#pragma omp parallel
	for(int i = 0; i< 10; ++i){
		#pragma omp single nowait
		doSingleStuff();
	}
}
