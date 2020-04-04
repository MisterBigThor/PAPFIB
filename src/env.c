#include <unistd.h>
#include "libminiomp.h"

// Global variable storing the ICV (internal control variables) supported in our implementation
miniomp_icv_t miniomp_icv;

// Function to parse OpenMP environment variables, currently just OMP_NUM_THREADS
void parse_env(void) {
    char * env = getenv ("OMP_NUM_THREADS");
    if (env == NULL) {
#if _DEBUG
      printf("LIB: Environment variable OMP_NUM_THREADS not defined, setting nthreads_var ICV to number of cores\n");
#endif
      int procs = (int)sysconf( _SC_NPROCESSORS_ONLN ); // returns the number of cores in system
      if (procs < 0) miniomp_icv.nthreads_var = 1;
      else miniomp_icv.nthreads_var = procs;
    } else {
      miniomp_icv.nthreads_var = atoi(env);
    }
#if _DEBUG
    printf("LIB: ICV nthreads_var = to %d\n", miniomp_icv.nthreads_var);
#endif

}
