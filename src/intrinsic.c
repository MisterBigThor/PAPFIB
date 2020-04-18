#include "libminiomp.h"

void omp_set_num_threads (int n) {
	miniomp_icv.nthreads_var = (n > 0 ? n : 1);
}

int omp_get_num_threads (void) {
	return(miniomp_icv.nthreads_var);
}

int omp_get_thread_num (void) {
	miniomp_parallel_t * aux = pthread_getspecific(miniomp_specifickey);
	return aux->id;
}

int omp_get_level (void) {
	miniomp_parallel_t * aux = pthread_getspecific(miniomp_specifickey);
   	return aux->nestedLevel;
}

