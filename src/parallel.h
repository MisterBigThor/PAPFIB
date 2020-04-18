#ifndef PARALLEL_H
#define PARALLEL_H

#include <pthread.h>
#include "list.h"

#define USED 1
#define UNUSED 0

extern pthread_t *miniomp_threads;

pthread_mutex_t parallelMutex;

struct threadStatus{
	int status;
};

struct threadStatus threadsStatus[MAX_THREADS];

typedef struct {
	void (*fn) (void *);
	void *fn_data;

	int id;
	int nestedLevel;
	int teamThreads;

	int num_threads;

	int barrierThread;
	pthread_barrier_t barrier;
	pthread_barrier_t *teamBarrier;

	//Sync stuff:
	pthread_mutex_t miniomp_default_lock;
	pthread_barrier_t miniomp_barrier;

} miniomp_parallel_t;

extern miniomp_parallel_t *miniomp_parallel;

extern pthread_key_t miniomp_specifickey;

extern pthread_key_t miniomp_internalkey;

void GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags);
void initParallel(void);
#endif
