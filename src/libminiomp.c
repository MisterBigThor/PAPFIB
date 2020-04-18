#include "libminiomp.h"
#include "intrinsic.h"

// Library constructor and desctructor
void init_miniomp(void) __attribute__((constructor));
void fini_miniomp(void) __attribute__((destructor));

// Function to parse OMP_NUM_THREADS environment variable
void parse_env(void);

void init_miniomp(void) {
	LOG ("mini-omp is being initialized\n");
	parse_env();

	miniomp_threads = malloc(MAX_THREADS * sizeof(pthread_t));
	miniomp_parallel = malloc(MAX_THREADS * sizeof(miniomp_parallel_t));
	miniomp_icv.nested_level = 0;

	pthread_key_create(&miniomp_specifickey, NULL);

	miniomp_parallel_t * miniomp_main = (miniomp_parallel_t *) malloc(sizeof(miniomp_parallel_t));
	miniomp_main->id = -1;
	miniomp_main->nestedLevel = 0;
	miniomp_main->num_threads = miniomp_icv.nthreads_var;
	pthread_mutex_init(&(miniomp_main->miniomp_default_lock), NULL);
	pthread_barrier_init(&(miniomp_main->miniomp_barrier),NULL, TEAM);
	pthread_setspecific(miniomp_specifickey, (void *) miniomp_main); // implicit initial pthread with id=-1

	initParallel();
	initSync();
	initMap();

	initSingle();
	initLoop();
	// Initialize OpenMP task queue for task and taskloop
}

void updateNumThreads(int numThreads){
	LOG("LIB: Refresh the num_threads with %i, for the default barrier.\n", numThreads);
	pthread_barrier_init(&miniomp_barrier, NULL, numThreads);
	return;
}

void fini_miniomp(void) {
	pthread_key_delete(miniomp_specifickey);

	destroyMap();
	clearSync();
	destroySingle();
	clearLoop();

	LOG("LIB: mini-omp is finalized\n");
}

