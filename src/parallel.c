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

void *worker(void *args) {
	miniomp_parallel_t *aux = args;
	#if _DEBUG
		printf("PARALLEL:Thread %d in worker\n", aux->id);
	#endif
	pthread_setspecific(miniomp_specifickey, (void *)aux);
	miniomp_parallel_t * aaa = pthread_getspecific(miniomp_specifickey);
	aux->fn(aaa->fn_data);
	pthread_exit(NULL);
	return(NULL);
}

void GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
	if(!num_threads) num_threads = omp_get_num_threads();

	#if _DEBUG
		printf("Starting: a parallel region using %d threads\n", num_threads);
		printf("Initialization of structures:\n");
	#endif

	for (int i=0; i<num_threads; i++){
		miniomp_parallel[i].id = i;
		miniomp_parallel[i].fn_data = data;
		miniomp_parallel[i].fn = fn;
//		miniomp_parallel[i].nestedLevel = n; internal variable
		pthread_create(&miniomp_threads[i], NULL, &worker,(void *) &miniomp_parallel[i]);
	}

	for(int i = 0; i < num_threads; i++){
		#if _DEBUG
			printf("Waiting for all team threads to finish.\n");
		#endif
		pthread_join(miniomp_threads[i], NULL);
	}

}

