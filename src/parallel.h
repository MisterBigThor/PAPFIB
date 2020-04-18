#ifndef PARALLEL_H
#define PARALLEL_H

#include <pthread.h>

extern pthread_t *miniomp_threads;

// Type declaration for parallel descriptor (arguments needed to create pthreads)
typedef struct {
	void (*fn) (void *);
	void *fn_data;
	int id;
	int used;
} miniomp_parallel_t;

extern miniomp_parallel_t *miniomp_parallel;

extern pthread_key_t miniomp_specifickey;

extern pthread_key_t miniomp_internalkey;

void GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags);

#endif
