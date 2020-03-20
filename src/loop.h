#ifndef _LOOP_H
#define _LOOP_H
// Type declaration for loop worksharing descriptor
typedef struct {
	bool inicialized;    	//constructor exists.

 	long start;           	// loop bounds and increment.
 	long end;
  	long incr;
  	int schedule;         	// schedule kind for loop.
  	long chunk_size;

	pthread_mutex_t mutexItDone;
	pthread_mutex_t	mutexNextIt;
	long itDone;	     	//its done so far.(data-race here)
	long nextIt;	    	//next iteration

  	int teamThreads;     	//implicit barrier:
	pthread_barrier_t barrier;

} miniomp_loop_t;

#define ws_STATIC 	0
#define ws_STATICCHUNK 	1
#define ws_DYNAMIC 	2
#define ws_GUIDED 	3
#define ws_RUNTIME 	4
#define ws_AUTO 	5

extern miniomp_loop_t miniomp_loop;

#endif
