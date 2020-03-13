#include "libminiomp.h"

/*
File for implement the barrier and the critical section (named & unnamed)
*/

// Default lock for critical sections
pthread_mutex_t miniomp_default_lock;

//UNAMED CRITICAL SECTION:
void GOMP_critical_start (void) {
	pthread_mutex_lock(&miniomp_default_lock);
//  	printf("TBI: Entering an unnamed critical, don't know if anyone else is alrady in. I proceed\n");
	#if _DEBUG
	printf("Locking the default lock for no-name critical\n");
	#endif
}

void GOMP_critical_end (void) {
	pthread_mutex_unlock(&miniomp_default_lock);
//  	printf("TBI: Exiting an unnamed critical section. I can not inform anyone else, bye!\n");
	#if _DEBUG
	printf("Unlocking the default lock for no-name critical\n");
	#endif
}

//NAMED CRITICAL SECTION:
void GOMP_critical_name_start (void **pptr) {
//	pthread_mutex_t * plock = *pptr;
	printf("name : %p\n", pptr);
	lockPosition(pptr);
}

void GOMP_critical_name_end (void **pptr) {
//	pthread_mutex_t * plock = *pptr;
	printf("name: %p, \n", pptr);
	unlockPosition(pptr);
//	printf("TBI: Exiting a named critical %p (%p), I can not inform anyone else, bye!\n", pptr, plock);
  	// if plock is still NULL something went wrong
}

//BARRIER:
pthread_barrier_t miniomp_barrier;

void GOMP_barrier() {
#if _DEBUG
	printf("GOMP_barrier: entering barrier \n");
#endif
//	printf("TBI: Entering in barrier, but do not know how to wait for the rest. I proceed\n");
	pthread_barrier_wait(&miniomp_barrier);
#if _DEBUG
	printf("GOMP_barrier: ended the barrier \n");
#endif
}
