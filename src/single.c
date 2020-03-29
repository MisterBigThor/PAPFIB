#include "libminiomp.h"
#include "errno.h"

// Declaratiuon of global variable for single work descriptor
miniomp_single_t miniomp_single;

bool GOMP_single_start (void){
	miniomp_single.singles[ID]++;
	int max = miniomp_single.max;
	if(miniomp_single.singles[ID] > max){
		return __sync_bool_compare_and_swap(&(miniomp_single.max), max, miniomp_single.singles[ID]);
	}
	return false;
}

void initSingle(void){
	miniomp_single.max = 0;
	for(int i = 0;i<MAX_THREADS; ++i) miniomp_single.singles[i] = 0;
	return;
}
void destroySingle(void){
	#if _DEBUG
		printf("	SINGLE STATS:		\n");
		printf("SINGLES REACHED: %u\n", miniomp_single.max);
		for(int i = 0; i<MAX_THREADS; ++i){
			printf("[%u] = %u ", i, miniomp_single.singles[i]);
			if((i % 4 == 0) & (i != 0)) printf("\n");
		}
		printf("\n");
		printf("	SINGLE STATS		\n");
	#endif
}
