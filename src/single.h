// Type declaration for single work sharing descriptor
typedef struct {
	bool singleActivation; 	//var for control the init.
	int idSingleThread; 	//only for info, usefull?
				//id for validate 2 single sections.
} miniomp_single_t;

// Declaration of global variable for single work descriptor
extern miniomp_single_t miniomp_single;
