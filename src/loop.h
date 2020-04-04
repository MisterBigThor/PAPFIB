#ifndef _LOOP_H
#define _LOOP_H

#include <list.h>


typedef struct {
	int initLoops;
	int reached[MAX_THREADS];
	pthread_mutex_t mutexLoop;
	struct list_head loopList;
}miniomp_loop;

struct loopDescr {
	long start;
	long end;
	long incr;
	int schedule;
	long chunk_size;

	int teamThreads;
	int threadInit;

	pthread_barrier_t barrier;
	pthread_mutex_t mutex;

	bool *myChunks;
	int sizeMyChunks;
	struct list_head anchor;
};

#define UNDEFINED 	-1
#define ws_STATIC 	0
#define ws_STATICCHUNK 	1
#define ws_DYNAMIC 	2
#define ws_GUIDED 	3
#define ws_RUNTIME 	4
#define ws_AUTO 	5



void initLoop(void);
void clearLoop(void);

#endif

