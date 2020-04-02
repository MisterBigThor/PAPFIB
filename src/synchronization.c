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
	#if _DEBUG && _DBGCRITICAL
	printf("Locking the default lock for no-name critical\n");
	#endif
}

void GOMP_critical_end (void) {
	pthread_mutex_unlock(&miniomp_default_lock);
	#if _DEBUG && _DBGCRITICAL
	printf("Unlocking the default lock for no-name critical\n");
	#endif
}

//NAMED CRITICAL SECTION:
void GOMP_critical_name_start (void **pptr) {
	lockPosition(pptr);
	#if _DEBUG && _DBGCRITICALNAME
	printf("(%u)CRITICAL_START: name : %p\n",omp_get_thread_num(), pptr);
	#endif


}

void GOMP_critical_name_end (void **pptr) {
	unlockPosition(pptr);
	#if _DEBUG && _DBGCRITICALNAME
	printf("(%u) CRITICAL_END: name : %p\n", omp_get_thread_num(), pptr);
	#endif
}

//BARRIER: Default barrier, inicialized by the num_threads of each parallel section.
pthread_barrier_t miniomp_barrier;

void GOMP_barrier() {
#if _DEBUG && _DBGBARRIER
	printf("(%u)GOMP_barrier: entering barrier \n", ID);
#endif
	pthread_barrier_wait(&miniomp_barrier);
#if _DEBUG && _DBGBARRIER
	printf("(%u)GOMP_barrier: ended the barrier \n", ID);
#endif
}
