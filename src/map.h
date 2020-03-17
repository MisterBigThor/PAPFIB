#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_MUTEXES 3

typedef struct {
	void * key;
	pthread_mutex_t plock;
} map;

extern map myMutexesMap[MAX_MUTEXES];

void initMap();
void destroyMap();

void lockPosition(void **key);
void unlockPosition(void **key);

void printMap();

