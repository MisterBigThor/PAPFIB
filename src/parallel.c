#include "libminiomp.h"

// This file implements the PARALLEL construct

// Declaration of array for storing pthread identifier from pthread_create function
pthread_t *miniomp_threads;

// Global variable for parallel descriptor
miniomp_parallel_t *miniomp_parallel;

// Declaration of per-thread specific key
pthread_key_t miniomp_specifickey;

// This is the prototype for the Pthreads starting function
void *worker(void *args) {
  // insert all necessary code here for:
	miniomp_parallel_t *aux = args;
#if _DEBUG
	printf("PARALLEL:Thread %d in worker\n", aux->id);
#endif
  //1) save thread-specific data
	pthread_setspecific(miniomp_specifickey, (void *)aux->fn_data);
  //2) invoke the per-threads instance of function encapsulating the parallel region
	aux->fn(pthread_getspecific(miniomp_specifickey));
  //3) exit the function
	pthread_exit(NULL);
	return(NULL);
}

 void GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
	if(!num_threads) num_threads = omp_get_num_threads();

#if _DEBUG
	printf("Starting: a parallel region using %d threads\n", num_threads);
	printf("Initialization of structures:\n");
#endif
//	miniomp_parallel = malloc(32 * sizeof(miniomp_parallel_t));
//	pthread_key_create(&miniomp_specifickey, NULL);
//	miniomp_threads = malloc(32 * sizeof(pthread_t));

	for (int i=0; i<num_threads; i++){
		miniomp_parallel[i].id = i;
		miniomp_parallel[i].fn_data = data;
		miniomp_parallel[i].fn = fn;
		pthread_create(&miniomp_threads[i], NULL, &worker,(void *) &miniomp_parallel[i]);
	}

	//Espera a que acabin tots els threads.
	for(int i = 0; i < num_threads; i++){
		#if _DEBUG
			printf("Waiting for all team threads to finish.\n");
		#endif
		pthread_join(miniomp_threads[i], NULL);
	}
}

