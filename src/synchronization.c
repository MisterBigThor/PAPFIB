#include "libminiomp.h"

/*
File for implement the barrier and the critical section (named & unnamed)
*/

// Default lock for critical sections
pthread_mutex_t miniomp_default_lock;

//UNAMED CRITICAL SECTION:
void GOMP_critical_start (void) {
	pthread_mutex_lock(&miniomp_default_lock);
	#if _DEBUG
	printMap();
	printf("Locking the default lock for no-name critical\n");
	#endif
}

void GOMP_critical_end (void) {
	pthread_mutex_unlock(&miniomp_default_lock);
	#if _DEBUG
	printMap();
	printf("Unlocking the default lock for no-name critical\n");
	#endif
}

//NAMED CRITICAL SECTION:
void GOMP_critical_name_start (void **pptr) {
	#if _DEBUG
	printMap();
	printf("CRITICAL: Critical name start with name : %p\n", pptr);
	#endif
	lockPosition(pptr);
}

void GOMP_critical_name_end (void **pptr) {
	#if _DEBUG
	printMap();
	printf("CRITICAL: Critical name ends with name: %p \n", pptr);
	#endif
	unlockPosition(pptr);
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
