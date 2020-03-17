#include "map.h"

map myMutexesPointer[MAX_MUTEXES];

pthread_mutexattr_t attr;

int keys; //keys used

void initMap(){
	#if _DEBUG
		printf("Key-value for mutex init \n");
	#endif
	keys = 0;
	for(int i = 0; i < MAX_MUTEXES; ++i){
		myMutexesPointer[i].key = NULL;
		pthread_mutex_init(&myMutexesPointer[i].plock, NULL);
	}
}

void destroyMap(){
	for(int i = 0; i < MAX_MUTEXES; ++i){
		pthread_mutex_destroy(&myMutexesPointer[i].plock);
	}
	#if _DEBUG
		printf("key-value for mutex ending, keys stored: %u \n", keys);
	#endif
}

void lockPosition(void ** pptr){
	for(int i = 0; i < MAX_MUTEXES; ++i){
		if(myMutexesPointer[i].key == pptr){
			pthread_mutex_lock(&myMutexesPointer[i].plock);
			#if _DEBUG
				printf("Locking key: %p", pptr );
			#endif
			return;
		}
	}
	for(int i = keys - 1; i < MAX_MUTEXES; ++i){
		if(myMutexesPointer[i].key == NULL){
			__sync_fetch_and_add(&keys, 1);
			myMutexesPointer[i].key = pptr;
			pthread_mutex_lock(&myMutexesPointer[i].plock);
			return;
		}
	}
}

void unlockPosition(void **pptr){
	for(int i = 0; i< MAX_MUTEXES; ++i){
		if(myMutexesPointer[i].key == pptr) {
			pthread_mutex_unlock(&myMutexesPointer[i].plock);
			#if _DEBUG
				printf("Unlocking key %p", pptr);
			#endif
			return;
		}
	}
	printf("something went wrong\n");
}

