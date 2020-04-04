#include "libminiomp.h"

#define MAX_NAMED_LOCKS 4

/*
File for implement the barrier and critical section (named & unnamed).
Uses a key-value, inicialized in the lib, for contain all the names and locks.
*/

//UNNAMED CRITICAL SECTION: Default lock for unnamed critical sections
pthread_mutex_t miniomp_default_lock;

void GOMP_critical_start (void) {
	pthread_mutex_lock(&miniomp_default_lock);
	LOG("Locking the default lock for no-name critical\n");
}

void GOMP_critical_end (void) {
	pthread_mutex_unlock(&miniomp_default_lock);
	LOG("Unlocking the default lock for no-name critical\n");
}

//NAMED CRITICAL SECTION:
void GOMP_critical_name_start (void **pptr) {
	lockPosition(pptr);
	LOG("(%u)CRITICAL_START: name : %p\n",omp_get_thread_num(), pptr);
}

void GOMP_critical_name_end (void **pptr) {
	unlockPosition(pptr);
	LOG("(%u) CRITICAL_END: name : %p\n", omp_get_thread_num(), pptr);
}

//BARRIER: Default barrier, inicialized by the num_threads of each parallel section.
pthread_barrier_t miniomp_barrier;

void GOMP_barrier() {
	LOG("(%u)GOMP_barrier: entering barrier \n", ID);
	pthread_barrier_wait(&miniomp_barrier);
	LOG("(%u)GOMP_barrier: ended the barrier \n", ID);
}
