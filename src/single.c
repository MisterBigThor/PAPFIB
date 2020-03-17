#include "libminiomp.h"

// Declaratiuon of global variable for single work descriptor
miniomp_single_t miniomp_single;

/* This routine is called when first encountering a SINGLE construct. 
   Returns true if this is the thread that should execute the clause.  */

bool GOMP_single_start (void)
{
	if(__sync_bool_compare_and_swap(&miniomp_single.singleActivation, false, true)){
		miniomp_single.idSingleThread = omp_get_thread_num();
		printf("SINGLE: YES!,I'm the single one \n");
		return(true);
	}
	else {
		printf("SINGLE: I'm not the single one :( \n");
		return(false);
	}
//  	printf("TBI: Entering into single, but don't know who else arrived so I proceed\n");

}


