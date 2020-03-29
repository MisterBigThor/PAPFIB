#ifndef _SINGLE_H
#define _SINGLE_H

#include "list.h"

// Type declaration for single work sharing descriptor
//we need a list to control if all threads enter the single.
//if a thread is in the list -> we have a new single!

typedef struct {
	struct list_head listSingle;
	int listSize[MAX_THREADS]; 	//listSize points to the 'last' single designation for each thread.
	pthread_mutex_t mutexSingle;	//mutex for an atomic add_to_list.
} miniomp_single_t;

struct listElement{
	unsigned int i; 		//number of thread that reach this single instance.
	char here[MAX_THREADS];		//indexed by num_thread
	struct list_head anchor;	//anchor to the next listElement.
};


// Declaration of global variable for single work descriptor
extern miniomp_single_t miniomp_single;


//inicialize a new Single control item, and appends to the list.
void initNewSingle(void);
void initAndAsignSingle(int id);
struct listElement * getNelement(int n);
#endif
