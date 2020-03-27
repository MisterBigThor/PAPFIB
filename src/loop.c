#include "libminiomp.h"


miniomp_loop_t miniomp_loop;

bool GOMP_loop_dynamic_next (long *istart, long *iend) {
	#if _DEBUG
		printf("(%u)LOOP: more iterations\n?", omp_get_thread_num());
	#endif
	//fill new iterations? return if still more it's.
//	*istart = NULL;
//	*iend = NULL;
	return(false);
}

/* The *_start routines are called when first encountering a loop construct
   that is not bound directly to a parallel construct.  The first thread
   that arrives will create the work-share construct; subsequent threads
   will see the construct exists and allocate work from it.

   START, END, INCR are the bounds of the loop; CHUNK_SIZE is the
   scheduling parameter.

   Returns true if there's any work for this thread to perform.  If so,
   *ISTART and *IEND are filled with the bounds of the iteration block
   allocated to this thread.  Returns false if all work was assigned to
   other threads prior to this thread's arrival.  */

bool GOMP_loop_dynamic_start (long start, long end, long incr, long chunk_size,
			long *istart, long *iend)
{
	if(__sync_bool_compare_and_swap(&miniomp_loop.inicialized, false, true)){
		miniomp_loop.start = start;
		miniomp_loop.end = end;
		miniomp_loop.incr = incr;
		miniomp_loop.chunk_size = chunk_size;

		pthread_mutex_init(&miniomp_loop.mutexItDone, NULL);
		pthread_mutex_init(&miniomp_loop.mutexNextIt, NULL);

		miniomp_loop.teamThreads = omp_get_num_threads();
		pthread_barrier_init(&miniomp_loop.barrier, NULL, miniomp_loop.teamThreads);//?

		miniomp_loop.nextIt = 0;
		#if _DEBUG
			printf("(%u)LOOP: init the descriptor.\n", omp_get_thread_num());
		#endif
	}

	bool ret = true;
	lock(miniomp_loop.mutexNextIt)
		*istart = miniomp_loop.nextIt;
		*iend = miniomp_loop.nextIt + miniomp_loop.chunk_size;
		long todo = miniomp_loop.end - miniomp_loop.nextIt;
		if(todo < 0) ret = false;
		else miniomp_loop.nextIt += miniomp_loop.chunk_size;
	unlock(miniomp_loop.mutexItDone)
	return ret;
}

void GOMP_loop_end (void) {
	#if _DEBUG
		printf("(%u)LOOP: loop_end -> waiting for the rest:\n", omp_get_thread_num());
	#endif
	pthread_barrier_wait(&miniomp_loop.barrier);
}

void GOMP_loop_end_nowait (void) {
	#if _DEBUG
		printf("(%u)LOOP: loop_end_nowait\n", omp_get_thread_num());
	#endif
	return;
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
