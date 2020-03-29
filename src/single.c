#include "libminiomp.h"
#include "errno.h"

// Declaratiuon of global variable for single work descriptor
miniomp_single_t miniomp_single;

bool GOMP_single_start (void){
	bool ret = false;
	struct listElement* actualSingle = getNelement(miniomp_single.listSize[ID]);
	//first time in this single?
	if(actualSingle->here[ID]) {
		int tl = pthread_mutex_trylock(&miniomp_single.mutexSingle);
		if(t1 == EBUSY) {
			ret = false; //ya han llegado y estan creando el nuevo single
			miniomp_single.pending[ID]++;
		}
		else if(t1 == 0 && (miniomop_single.maxSingle == actualSingle.listSize[ID]){ //soy el primero en llegar?
			actualSingle.maxSingle++;
			initAndAsingSingle(ID);
			ret = true;
		}
		else {
			
			ret = false;
		}
		unlock(miniomp_single.mutexSingle);
		return true;
	}
	//this thread didn't pass this single, let's see if is the first one:
	else if(__sync_add_and_fetch(&(actualSingle->i), 1) == 1) {
		#if _DEBUG
		printf("(%u)Soy el elegido para el single\n", ID);
		#endif
		ret = true;
	}
	actualSingle->here[ID] = true;
	return ret;
}

void initSingle(void){
	INIT_LIST_HEAD(&miniomp_single.listSingle);
	pthread_mutex_init(&miniomp_single.mutexSingle, NULL);
	struct listElement * ret = malloc(sizeof(struct listElement));
	ret->i = 0;
	list_add_tail(&(ret->anchor),&miniomp_single.listSingle);
	miniomp_single.maxSingle = 1;
}
void destroySingle(void){
	#if _DEBUG
	pritnf("single reached by the threads: %u\n", miniomp_single.maxSingle);
	for(int i = 0; i<MAX_THREADS; ++i){
		printf("Singles in %u %u\n", i, miniomp_single.listSize[i]);
	}
	#endif
}
void initAndAsignSingle(int id){
	#if _DEBUG
	printf("(%u)Init a new SingleInstance.\n", ID);
	#endif
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
