#include "libminiomp.h"
#include "intrinsic.h"

// Library constructor and desctructor
void init_miniomp(void) __attribute__((constructor));
void fini_miniomp(void) __attribute__((destructor));

// Function to parse OMP_NUM_THREADS environment variable
void parse_env(void);

void init_miniomp(void) {
	#if _DEBUG
	printf ("mini-omp is being initialized\n");
	#endif

	// Parse OMP_NUM_THREADS environment variable to initialize nthreads_var internal control variable
	parse_env();

	miniomp_threads = malloc(MAX_THREADS * sizeof(pthread_t));
	miniomp_parallel = malloc(MAX_THREADS * sizeof(miniomp_parallel_t));

	pthread_key_create(&miniomp_specifickey, NULL);
	pthread_setspecific(miniomp_specifickey, (void *) 0); // implicit initial pthread with id=0

	// Initialize OpenMP default lock and default barrier
	printf("Initialized default OMP barrier with %u \n", miniomp_icv.nthreads_var);
	pthread_barrier_init(&miniomp_barrier, NULL, miniomp_icv.nthreads_var);
	pthread_mutex_init(&miniomp_default_lock, NULL);
	initMap(); //inint support for mutex-named critical

	// Initialize OpenMP workdescriptors for for and single
	miniomp_single.singleActivation = false;		//initialize the single descritptor with a null value.
	miniomp_loop.inicialized = false;	 		//initialize the loop descriptor with a null value.
	// Initialize OpenMP task queue for task and taskloop

}

void updateNumThreads(int numThreads){
	#if _DEBUG
		printf("Refresh the num_threads");
	#endif
	miniomp_icv.nthreads_var = numThreads;
	pthread_barrier_init(&miniomp_barrier, NULL, numThreads);
	return;
}

void fini_miniomp(void) {
	// delete Pthread thread-specific data
	pthread_key_delete(miniomp_specifickey);

	// free other data structures allocated during library initialization
	pthread_mutex_destroy(&miniomp_default_lock);
	pthread_barrier_destroy(&miniomp_barrier); // free default barrier

	destroyMap();

	printf ("mini-omp is finalized\n");
}

