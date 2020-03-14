#include "map.h"

struct map myMutexesPointer[MAX_MUTEXES];
int freeMutexMap;
//struct map * myMutexesPointer;
pthread_mutexattr_t attr;

void initMap(){
	//printf("inicializacion de map de mutexes \n");
//	myMutexesPointer = malloc(sizeof(struct map) * MAX_MUTEXES);
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
	pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
	for(int i = 0; i < MAX_MUTEXES; ++i){
		myMutexesPointer[i].key = NULL;
		pthread_mutex_init(&myMutexesPointer[i].plock, &attr);
	}
	//printf("fin de inicializacion del map\n");
}

void destroyMap(){
//	printf("llamada a destroy");
	for(int i = 0; i < MAX_MUTEXES; ++i){
		pthread_mutex_destroy(&myMutexesPointer[i].plock);
	}
}

void lockPosition(void ** pptr){
	for(int i = 0; i < MAX_MUTEXES; ++i){
		if(myMutexesPointer[i].key == *pptr){
	//		printf("locking...");
			pthread_mutex_lock(&myMutexesPointer[i].plock);
			myMutexesPointer[i].key = pptr;
			#if _DEBUG
			printf("Locking key: %p", pptr );
			#endif
			return;
		}
	}
	for(int i = 0; i < MAX_MUTEXES; ++i){
		if(myMutexesPointer[i].key == NULL){
			myMutexesPointer[i].key = pptr;
			pthread_mutex_lock(&myMutexesPointer[i].plock);
			return;
		}
	}
}

void unlockPosition(void **pptr){
	for(int i = 0; i< MAX_MUTEXES; ++i){
		if(myMutexesPointer[i].key == *pptr) {
	//		printf("unlock");
			pthread_mutex_unlock(&myMutexesPointer[i].plock);
			myMutexesPointer[i].key = NULL;
			#if _DEBUG
			printf("Unlocking key %p", pptr);
			#endif
			return;
		}
	}
}

void printMap(){
	for(int i = 0; i< MAX_MUTEXES; ++i){
		if(myMutexesPointer[i].key != NULL)
			printf("%d: %p \n", i, myMutexesPointer[i].key);
	}
}
