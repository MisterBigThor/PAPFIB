#ifndef ENV_H
#define ENV_H
// Type declaration for Internal Control Variables (ICV) structure
typedef struct {
	int nthreads_var;
	// Add other ICVs if needed
} miniomp_icv_t;

extern miniomp_icv_t miniomp_icv;

void parse_env(void);

#endif
