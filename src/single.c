#include "libminiomp.h"
#include "errno.h"

// Declaratiuon of global variable for single work descriptor
miniomp_single_t miniomp_single;

bool GOMP_single_start (void){
	struct listElement* actualSingle = getNelement(miniomp_single.listSize[ID]);

	if(actualSingle->here[ID]) { // ya he pasado por aqui, nueva isntancia de single:
		#if _DEBUG
		printf("(%u)Ya he pasado por este single\n", ID);
		#endif
		initAndAsignSingle(ID);
		return true;
	}
	//this thread didn't pass this single, let's see if is the first one:
	//in the two cases, ++listSize;
	unsigned int r = __sync_add_and_fetch(&(actualSingle->i), 1);
//	printf("%x -> %u\n",&(actualSingle->i) ,r);
	if(r == 1) {
		#if _DEBUG
		printf("(%u)Soy el elegido para el single\n", ID);
		#endif
		actualSingle->here[ID] = true;
		return true;
	}
	else {
		#if _DEBUG
		printf("(%u)No soy el elegido\n", ID);
		#endif
		actualSingle->here[ID] = true;
		return false;
	}
	printf("Problems!!!!\n");
	return false;
}


void initSingle(void){
	INIT_LIST_HEAD(&miniomp_single.listSingle);
	pthread_mutex_init(&miniomp_single.mutexSingle, NULL);
	struct listElement * ret = malloc(sizeof(struct listElement));
	ret->i = 0;
	list_add_tail(&(ret->anchor),&miniomp_single.listSingle); //'chronologic'
}
//this operation needs to be atomic!
void initAndAsignSingle(int id){
	printf("init a new Single.\n");
	struct listElement * ret = malloc(sizeof(struct listElement));
	ret->i = 1;
	list_add_tail(&(ret->anchor),&miniomp_single.listSingle);
	ret->here[id] = 1;
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
