#include "libminiomp.h"
#include <errno.h>

miniomp_loop_t *miniomp_loop;
void initDescriptor(long start, long end, long incr, long chunk_size);
bool allocateIterations(long *istart, long *iend);


bool GOMP_loop_dynamic_start (long start, long end, long incr, long chunk_size, long *istart, long *iend){
	LOG("(%u)LOOP: Dynamic start\n", ID);
	if(miniomp_loop->init != 0) {
		miniomp_loop->init++;
		return allocateIterations(istart, iend);
	}
	int tl;
	while((tl = pthread_mutex_trylock(&miniomp_loop->mutexMyChunks))==EBUSY){}
	if((tl == 0) & (miniomp_loop->init== 0)){
		LOG("(%u)LOOP: inicializando el descriptor de loop.\n", ID);
		miniomp_loop->init++;
		initDescriptor(start, end, incr, chunk_size);
		unlock(miniomp_loop->mutexMyChunks)
		bool ret = allocateIterations(istart, iend);
		return ret;
	}
	if((tl == 0) & (miniomp_loop->init != 0)){
		miniomp_loop->init++;
		unlock(miniomp_loop->mutexMyChunks)
		return allocateIterations(istart,iend);
	}
	LOG("(%u)PANIC! dyn_start", ID);
	return false;
}

void GOMP_loop_end (void) {
	lock(miniomp_loop->mutexMyChunks)
	miniomp_loop->init--;
	unlock(miniomp_loop->mutexMyChunks)
	LOG("(%u)LOOP: loop end\n", ID);
	pthread_barrier_wait(&miniomp_loop->barrier);
}

void GOMP_loop_end_nowait (void) {
	lock(miniomp_loop->mutexMyChunks)
	miniomp_loop->init--;
	unlock(miniomp_loop->mutexMyChunks)
	LOG("(%u)LOOP: nowait end\n", ID);
	return;
}

void initLoop(void){
	LOG("LOOP: init data & structures\n");
	miniomp_loop = malloc(sizeof(miniomp_loop_t));
	miniomp_loop->init = 0;
	miniomp_loop->initLoops = 0;
	pthread_mutex_init(&miniomp_loop->mutexMyChunks, NULL);
	pthread_barrier_init(&miniomp_loop->barrier, NULL, 1);
}
void clearLoop(void){
	LOG("LOOP: clear data & structures\n");
	pthread_mutex_destroy(&miniomp_loop->mutexMyChunks);
	pthread_barrier_destroy(&miniomp_loop->barrier);
	LOG("LOOP: loops reached: %i\n", miniomp_loop->initLoops);
	LOG("LOOP: loop cntr : %i\n", miniomp_loop->init);
	free(miniomp_loop);
}

bool GOMP_loop_dynamic_next (long *istart, long *iend) {
	return allocateIterations(istart, iend);
}

void initDescriptor(long start, long end, long incr, long chunk_size){
	miniomp_loop->init++;
	int l = (end-start)/chunk_size;
	int r = (end-start)%chunk_size;
	if(r != 0) l++;
	LOG("(%u)LOOP: init descriptor for %i chunks\n",ID, l);
	miniomp_loop->sizeMyChunks = l;
	miniomp_loop->myChunks = malloc (sizeof(bool)*l);
	miniomp_loop->start = start;
	miniomp_loop->end = end;
	miniomp_loop->incr = incr;
	miniomp_loop->chunk_size = chunk_size;
	miniomp_loop->schedule = ws_DYNAMIC;
	miniomp_loop->teamThreads = omp_get_num_threads();
	pthread_barrier_init(&miniomp_loop->barrier, NULL, miniomp_loop->teamThreads);
	for(int i = 0; i < l; ++i) miniomp_loop->myChunks[i] = false;
	miniomp_loop->initLoops++;
}
bool allocateIterations(long *istart, long *iend){
	lock(miniomp_loop->mutexMyChunks)
	if(miniomp_loop->init == 0) printf("(%u)LOOP: PANIC!\n",ID);
	int i = 0;
	while(i < miniomp_loop->sizeMyChunks){
		if(!miniomp_loop->myChunks[i]){
			*istart = miniomp_loop->start + (i*miniomp_loop->chunk_size);
			*iend = *istart+miniomp_loop->chunk_size;
			miniomp_loop->myChunks[i] = true;
			LOG("(%u)LOOP: assigned [%li, %li]\n",ID, *istart, *iend);
			unlock(miniomp_loop->mutexMyChunks)
			return true;
		}
		++i;
	}
	LOG("(%u)No more iterations left\n", ID);
	unlock(miniomp_loop->mutexMyChunks)
	return false;
}

#if 0
// Only implement this if really needed, i.e. you find a case in which it is invoked

/* The GOMP_parallel_loop_* routines pre-initialize a work-share construct
   to avoid one synchronization once we get into the loop. The compiler 
   does not invoke the *_start routine for the work-share. And of course, 
   the compiler does not invoke GOMP_loop_end. These routines should create 
   the team of threads to execute the work-share in parallel */

void
GOMP_parallel_loop_dynamic (void (*fn) (void *), void *data,
                            unsigned num_threads, long start, long end,
                            long incr, long chunk_size, unsigned flags)
{
  printf("TBI: What another mess! Directly starting a parallel and a non-static for worksharing construct! I am lost!\n");
  // Here you should pre-initialize the work-sharing structures as done in 
  // GOMP_loop_dynamic_start; only the master thread is doing this, the other
  // threads in the team do not reach this point 
  GOMP_parallel (fn, data, num_threads, flags);
}

#endif
