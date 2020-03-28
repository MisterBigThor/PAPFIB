#include "libminiomp.h"

// Declaratiuon of global variable for single work descriptor
miniomp_single_t miniomp_single;

/* This routine is called when first encountering a SINGLE construct. 
   Returns true if this is the thread that should execute the clause.  */
bool GOMP_single_start (void){
	struct listElement* actualSingle = getNelement(miniomp_single.listSize[ID]);
	if(actualSingle->here[ID]) { // ya he pasado por aqui, nueva isntancia de single:
		printf("(%u)Ya he pasado por este single\n", ID);
		initAndAsignSingle(ID); //atomic here!
		return true;
	}
	//this thread didn't pass this single, let's see if is the first one:
	//in the two cases, ++listSize;
	else if((__sync_fetch_and_add(&(actualSingle->i), 1))== 1) {
		printf("(%u)Soy el elegido para el single\n", ID);
		actualSingle->here[ID] = true;
		miniomp_single.listSize[ID]++;
		return true;
	}
	else {
		printf("(%u)No soy el elegido\n", ID);
		miniomp_single.listSize[ID]++;
		return false;
	}
	printf("Problems!!!!\n");
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

//list_entry(ptr,struct listElement, anchor);
//return the n element in the list.
struct listElement * getNelement(int n){
	struct list_head * list = list_first(&miniomp_single.listSingle); 
	while(n > 0){
		list = list->next;
		n--;
	}
	struct listElement * ret = list_entry(list, struct listElement, anchor);
	return ret;
}



