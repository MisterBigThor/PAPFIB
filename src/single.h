#ifndef _SINGLE_H
#define _SINGLE_H

#include "list.h"

// Type declaration for single work sharing descriptor

typedef struct {
	bool singleActivation; 	//var for control the init.ONLY works with one single

	//we need a list to control if all threads enter the single.
	//if a thread is in the list -> we have a new single!
	struct list_head listSingle;
	int listSize;

} miniomp_single_t;

struct listElement{
	unsigned int i; 	//number of single reached.
	bool here[MAX_THREADS];	//indexed by num_thread
	struct list_head anchor;//anchor to the next listElement.
};


// Declaration of global variable for single work descriptor
extern miniomp_single_t miniomp_single;

#endif
