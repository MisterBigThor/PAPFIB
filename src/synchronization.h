#include <pthread.h>

// Default lock for unnamed critical sections
extern pthread_mutex_t miniomp_default_lock;

// Default barrier within a parallel region
extern pthread_barrier_t miniomp_barrier;

// Named locks struct, as a list?
typedef struct{
	void * key;
	pthread_mutex_t * lock;
} namedLock;

extern namedLock *myLockList;

// Functions implemented in this module

void critical_names_init(void);
void critical_names_end(void);

void GOMP_critical_start (void);
void GOMP_critical_end (void);
void GOMP_critical_name_start (void **pptr);
void GOMP_critical_name_end (void **pptr);
void GOMP_barrier(void);
