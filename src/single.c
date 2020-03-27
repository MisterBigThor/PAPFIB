#include "libminiomp.h"

// Declaratiuon of global variable for single work descriptor
miniomp_single_t miniomp_single;

/* This routine is called when first encountering a SINGLE construct. 
   Returns true if this is the thread that should execute the clause.  */




bool GOMP_single_start (void){
//thread mantiene una cuenta 'privada' de cuantos singles ha atravesado.
//aceder a la ultima posicion de la lista donde este el single.

	int id = omp_get_thread_num();
	struct listElement* actualSingle = getNelement(miniomp_single.listSize[id]);
	if(actualSingle->here[id]) {
		//atomic:
		initAndAsignSingle(id);
		return true;
	}
	//this thread didn't pass this single, let's see if is the first one:
	else if((__sync_fetch_and_add(&(actualSingle->i), 1))== 1) {

		actualSingle->here[id] = true;
		return true;

	}

	else {

		return false;
	}
/*
	if(__sync_bool_compare_and_swap(&miniomp_single.singleActivation, false, true)){
		miniomp_single.idSingleThread = omp_get_thread_num();
		#if _DEBUG
		printf("SINGLE: YES!,I'm the single one \n");
		#endif
		return(true);
	}
	else {
		#if _DEBUG
		printf("SINGLE: I'm not the single one :( \n");
		#endif
		return(false);
	}
*/
	return false;
}


void initNewSingle(void){
	struct listElement ret;
	ret.i = 0;
	list_add_tail(&(ret.anchor),&miniomp_single.listSingle); //'chronologic'
}
//this operation needs to be atomic!
void initAndAsignSingle(int id){
	struct listElement ret;
	ret.i = 1;
	list_add_tail(&(ret.anchor),&miniomp_single.listSingle);
	ret.here[id] = 1; //this is the thread that will run the single.
	miniomp_single.listSize[id]++;
}
struct listElement * getNelement(int n){
	//TBI: dema?
	return NULL;
}



