#include <stdio.h>
#include <stdlib.h>
#include <omp.h>	/* OpenMP */

int first=0, second=0;

int foo() {
    int i, x = 1023;
    #pragma omp parallel firstprivate(x) reduction(+:first) if(x>0) num_threads(5)
    {
	    x++; 
	    first += x;
    }
    printf("Thread finished first parallel with %i, %i \n", first, x);
    #pragma omp parallel firstprivate(x) reduction(+:first) num_threads(4)
    {
	    x++; 
	    first += x;
    }
    printf("Thread finished second parallel with %i, %i \n", first, x);
    #pragma omp parallel private(i) shared(first) reduction(+:second)
    {
	second = first;
	for (i = 0; i < 16; i++) second++;
    }
    omp_set_num_threads(6);
    printf("Thread finished third parallel with %i.\n", second);

    #pragma omp parallel
    printf("Thread %d finished the execution of foo\n", omp_get_thread_num());
    printf("All finished\n");
    return(x);
}

int main(int argc, char *argv[]) {
    printf("first = %d, second = %d, x = %d\n", first, second, foo());
}
