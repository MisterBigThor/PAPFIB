#include "map.h"

struct map myMutexesMap[MAX_MUTEXES];
int freeMutexMap;

void initMap(){
	printf("inicializacion de map de mutexes \n");
	//freeMutexMap = 0;
	//initialize all mutex?
	for(int i = 0; i < MAX_MUTEXES; ++i){
		myMutexesMap->key = NULL;
	}
//	freeMutexMap = MAX_MUTEXES;
}

void destroyMap(){

}

int findKeyPosition(void ** key){
	for(int i = 0; i<MAX_MUTEXES; ++i){
		if(myMutexesMap->key == key) return i;
	}
	return -1;
}

void lockPosition(void ** pptr){
	int firstFree = -1;
	for(int i = 0; i < MAX_MUTEXES; ++i){
		if(myMutexesMap[i].key == pptr){
			pthread_mutex_init(&myMutexesMap[i].plock, NULL);
			pthread_mutex_lock(&myMutexesMap[i].plock);
			myMutexesMap[i].key = pptr;
			#if _DEBUG
			printf("Locking key: %p", pptr );
			#endif
			return;
		}
		if(myMutexesMap[i].key == NULL) firstFree=i;
	}
	pthread_mutex_init(&myMutexesMap[firstFree].plock, NULL);
	myMutexesMap[firstFree].key = pptr;
	pthread_mutex_lock(&myMutexesMap[firstFree].plock);
}
void unlockPosition(void **pptr){
	for(int i = 0; i< MAX_MUTEXES; ++i){
		if(myMutexesMap[i].key == pptr) {
			pthread_mutex_unlock(&myMutexesMap[i].plock);
			myMutexesMap[i].key = NULL;
			#if _DEBUG
			printf("Unlocking key %p", pptr);
			#endif
		}
	}
}

void printMap(){
	for(int i = 0; i< MAX_MUTEXES; ++i){
		if(myMutexesMap[i].key != NULL)
			printf("%d: %p \n", i, myMutexesMap[i].key);
	}
}
