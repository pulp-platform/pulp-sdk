#define SW_DATA_SIZE 14
#define N_CORES 8

PI_L1 int A[SW_DATA_SIZE];
PI_L1 int B[SW_DATA_SIZE];
PI_L1 int C[SW_DATA_SIZE];
void initialize_mat(int core_id){
	for(int i=core_id-1; i<SW_DATA_SIZE; i=i+N_CORES-1)
	{
		A[i] = i;//initialize data in matrix same as the index
		B[i] = i;//initialize data in matrix same as the index
	}
}

void add_mat(int core_id){
	for(int i=core_id-1; i<SW_DATA_SIZE; i=i+N_CORES-1)
	{
		C[i] = A[i]+B[i];//Addition of matrix A and B and the result is stored into C
	}
}

/*Print functionality to debug*/
void print_mat(int core_id){
	for(int i=core_id-1; i<SW_DATA_SIZE; i=i+N_CORES-1)
	{
		printf("A[%d]=%d, B[%d]=%d, C[%d]=%d using core=%d\n", i, A[i], i, B[i], i, C[i], core_id);
	}
}