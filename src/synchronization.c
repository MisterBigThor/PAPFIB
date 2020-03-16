#include "libminiomp.h"

#define MAX_NAMED_LOCKS 4

/*
File for implement the barrier and the critical section (named & unnamed)
*/

// Default lock for critical sections
pthread_mutex_t miniomp_default_lock;

//namedLock * myLockList;
//namedLock myNamedLocks[MAX_NAMED_LOCKS];

/*
void critical_names_init(void){
	printf("starting names:\n");
	myLockList = malloc(MAX_NAMED_LOCKS * sizeof(namedLock));
	for(int i = 0; i<MAX_NAMED_LOCKS; ++i){
		printf("i: %d\n", i);
		myLockList[i].key = NULL;
		pthread_mutex_init(myLockList[i].lock, NULL);
	}
	printf("done init\n");
	return;
}
void critical_names_end(void){
	for(int i = 0; i<MAX_NAMED_LOCKS; ++i){
		pthread_mutex_destroy(myLockList[i].lock);
	}
	free(myLockList);
	return;
}

inline void lockPosition(void **pptr){
	for(int i = 0; i < MAX_NAMED_LOCKS; i++){
		if(myLockList[i].key == pptr){
			pthread_mutex_lock(myLockList[i].lock);
		}
	}
	for(int i = 0; i < MAX_NAMED_LOCKS; i++){
		if(myLockList[i].key == NULL){
			myLockList[i].key = pptr;
			pthread_mutex_lock(myLockList[i].lock);
		}
	}
}

inline void unlockPosition(void **pptr){
	for(int i = 0; i < MAX_NAMED_LOCKS; i++){
		if(myLockList[i].key == pptr){
			pthread_mutex_lock(myLockList[i].lock);
			myLockList[i].key = NULL;
			return;
		}
	}
	printf("Error: unlocking a mutex not previous locked");
}
*/
//UNAMED CRITICAL SECTION:
void GOMP_critical_start (void) {
	pthread_mutex_lock(&miniomp_default_lock);
	#if _DEBUG
	printf("Locking the default lock for no-name critical\n");
	#endif
}

void GOMP_critical_end (void) {
	pthread_mutex_unlock(&miniomp_default_lock);
	#if _DEBUG
	printf("Unlocking the default lock for no-name critical\n");
	#endif
}

//NAMED CRITICAL SECTION:

void GOMP_critical_name_start (void **pptr) {
//	lockPosition(pptr);
	lockPosition(pptr);
	#if _DEBUG
	printf("CRITICAL: Critical name start with name : %p\n", pptr);
	#endif


}

void GOMP_critical_name_end (void **pptr) {
//	unlockPosition(pptr);
	unlockPosition(pptr);
	#if _DEBUG
	printf("CRITICAL: Critical name ends with name: %p \n", pptr);
	#endif
}

//BARRIER:
pthread_barrier_t miniomp_barrier;

void GOMP_barrier() {
#if _DEBUG
	printf("GOMP_barrier: entering barrier \n");
#endif
	pthread_barrier_wait(&miniomp_barrier);
#if _DEBUG
	printf("GOMP_barrier: ended the barrier \n");
#endif
}
