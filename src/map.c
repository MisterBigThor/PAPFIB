#include "map.h"

map * myMutexesPointer;

int keys; //keys used
int size; //actual array size

void initMap(){
	#if _DEBUG && _DBGMAP
		printf("Key-value for mutex init.\n");
	#endif
	myMutexesPointer = malloc(sizeof(map)*INIT_MUTEXES);
	keys = 0;
	size = INIT_MUTEXES;
	for(int i = 0; i < INIT_MUTEXES; ++i){
		myMutexesPointer[i].key = NULL;
		pthread_mutex_init(&myMutexesPointer[i].plock, NULL);
	}
}

void destroyMap(){
	for(int i = 0; i < size; ++i){
		pthread_mutex_destroy(&myMutexesPointer[i].plock);
	}
	#if _DEBUG &&_DBGMAP
		printf("key-value for mutex ending, keys stored: %u \n", keys);
	#endif
}

void lockPosition(void ** pptr){
	for(int i = 0; i < keys; ++i){
		if(myMutexesPointer[i].key == pptr){
			pthread_mutex_lock(&myMutexesPointer[i].plock);
			#if _DEBUG &&_DBGMAP
				printf("Locking key: %p", pptr );
			#endif
			return;
		}
	}
	for(int i = 0; i < size; ++i){
		if(myMutexesPointer[i].key == NULL){
			#if _DEBUG &&_DBGMAP
				printf("NEW LOCKING!");
			#endif
			__sync_fetch_and_add(&keys, 1);
			myMutexesPointer[i].key = pptr;
			pthread_mutex_lock(&myMutexesPointer[i].plock);
			return;
		}
	}
}

void unlockPosition(void **pptr){
	for(int i = 0; i < size; ++i){
		if(myMutexesPointer[i].key == pptr) {
			pthread_mutex_unlock(&myMutexesPointer[i].plock);
			#if _DEBUG && _DBGMAP
				printf("Unlocking key %p", pptr);
			#endif
			return;
		}
	}
	printf("something went wrong!!!\n");
	exit(-1);
}

