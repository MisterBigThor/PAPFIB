#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_MUTEXES 4

static struct map{
	void * key;
	static pthread_mutex_t plock;
};

extern static struct map myMutexesMap[MAX_MUTEXES];


void initMap();
void destroyMap();

void lockPosition(void **key);
void unlockPosition(void **key);

void printMap();

