#include <stdio.h>
#include <pthread.h>

#define MAX_MUTEXES 50


struct map{
	void ** key;
	pthread_mutex_t plock;
};

extern struct map myMutexesMap[MAX_MUTEXES];
extern int freeMutexMap;

void initMap();
void destroyMap();

void lockPosition(void **key);
void unlockPosition(void **key);

void printMap();

