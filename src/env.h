#ifndef ENV_H
#define ENV_H

typedef struct {
	int nthreads_var;
	int threads_in_use;
	int nested_level;
} miniomp_icv_t;

extern miniomp_icv_t miniomp_icv;

void parse_env(void);

#endif
