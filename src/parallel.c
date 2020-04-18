#include "libminiomp.h"

/*
File for implement the Parallel constructor.
It used an array of pthreads_t, a struct to control the worker of all threads,
and specific key to allocate per thread data.
ToDo: nestedLevel
*/

pthread_t *miniomp_threads;
miniomp_parallel_t *miniomp_parallel;
pthread_key_t miniomp_specifickey;


//cond vars?

void *worker(void *args) {
	miniomp_parallel_t *aux = args;
	pthread_setspecific(miniomp_specifickey, (void *)aux);
	miniomp_parallel_t * aaa = pthread_getspecific(miniomp_specifickey);
	LOG("(%u)PARALLEL: worker\n", ID);
	aux->fn(aaa->fn_data);
	pthread_exit(NULL);
	return NULL;
}

void GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
	if(!num_threads) num_threads = omp_get_num_threads();
	else updateNumThreads(num_threads);

	LOG("PARALLEL: starting a parallel region :%d threads\n", num_threads);
	miniomp_icv.nested_level++;
	LOG("PARALLEL: actual nestedLevel: %d\n", miniomp_icv.nested_level);

	for (int i=0; i<num_threads; i++){
		miniomp_parallel[i].id = i;
		miniomp_parallel[i].fn_data = data;
		miniomp_parallel[i].fn = fn;
		miniomp_parallel[i].used = USED;
		miniomp_parallel[i].nested_level = miniomp_icv.nested_level;
		pthread_create(&miniomp_threads[i], NULL, &worker,(void *) &miniomp_parallel[i]);
	}
	for(int i = 0; i < num_threads; i++){
		pthread_join(miniomp_threads[i], NULL);
	}
	miniomp_icv.nested_level--;

}

