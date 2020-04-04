#ifndef _LOOP_H
#define _LOOP_H
// Type declaration for loop worksharing descriptor
typedef struct {
	int init;    	//constructor exists.
	
 	long start;           	// loop bounds and increment.
 	long end;
  	long incr;
  	int schedule;         	// schedule kind for loop.
  	long chunk_size;

  	int teamThreads;     	//implicit barrier:
	pthread_barrier_t barrier;

	pthread_mutex_t mutexMyChunks; //data race over myChunks;
	bool *myChunks;
	int sizeMyChunks;

	int initLoops;
} miniomp_loop_t;

#define ws_STATIC 	0
#define ws_STATICCHUNK 	1
#define ws_DYNAMIC 	2
#define ws_GUIDED 	3
#define ws_RUNTIME 	4
#define ws_AUTO 	5

extern miniomp_loop_t * miniomp_loop;

void initLoop(void);
void clearLoop(void);

#endif

