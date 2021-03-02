/* 
 * Copyright (C) 2020 University of Bologna
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 * Authors: Manuele Rusci, UniBO (manuele.rusci@unibo.it)
 */

#include "pmsis.h"
#include <bsp/bsp.h>

//#define PRINT_MATRIX 
#define PERFORMANCE
#define N 500

// global variables
int A[N];   
int B[N];
#ifndef DOUBLE_BUFFERING
int tempC[N];
#else
int tempC[2*N];
static struct pi_task ram_write_tasks[N];
static int count = 0;
// Callback for asynchronous ram write
static void end_of_tx(void *arg)
{
  //printf("End of %d TX \n", count);
  count++;
}
#endif
static uint32_t hyper_buff; // result on L3

// global struct define
static struct pi_device ram;
static struct pi_hyperram_conf conf;


// matrix functions
void task_initMat()
{
    for(int i=0;i<N;i++){
        A[i] = i;
        B[i] = i;
    }
}

void task_VectProdScalar(int scalarA, int* matB, int * matC, int dim)
{
    for(int i=0;i<dim;i++){
        matC[i] = scalarA * matB[i];
    }
}

void print_matrix(int * mat, int dim)
{
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            printf("%02d ", mat[i*dim+j]);
        }
        printf("\n");
    }
}

int main()
{
    /* Init & open ram. */
    pi_hyperram_conf_init(&conf);
    pi_open_from_conf(&ram, &conf);
    if (pi_ram_open(&ram))
    {
        printf("Error ram open !\n");
        pmsis_exit(-3);
    }

    if (pi_ram_alloc(&ram, &hyper_buff, N*N*sizeof(int) ))
    {
        printf("Ram malloc failed !\n");
        pmsis_exit(-4);
    }

    // initialize matrix operands
    task_initMat();

#ifdef PRINT_MATRIX
    printf("\n\nThis is the Matrix A\n");
    print_matrix(A, N);
    printf("\n\nThis is the Matrix B\n");
    print_matrix(B, N);
#endif
#ifdef PERFORMANCE
    //initialize performance counters
    pi_perf_conf(
        1 << PI_PERF_CYCLES | 
        1 << PI_PERF_INSTR 
    );

    // measure statistics on matrix operations
    pi_perf_reset();
    pi_perf_start();
#endif
#ifndef DOUBLE_BUFFERING
    for(int i=0; i<N;i++){
        task_VectProdScalar(A[i], B, tempC, N );
        pi_ram_write(&ram, hyper_buff+i*N*sizeof(int), tempC, (uint32_t) N* sizeof(int));
    }
#else
    int i=1;
    int buffer_id;
    task_VectProdScalar(A[0], B, tempC, N );
    for(i; i<N;i++){
	buffer_id = i & 0x1;
        pi_ram_write_async(&ram, hyper_buff+i*N*sizeof(int), &tempC[N*(1-buffer_id)], (uint32_t) N * sizeof(int), pi_task_callback(&ram_write_tasks[i-1], end_of_tx, NULL));
        task_VectProdScalar(A[i], B, &tempC[N*buffer_id], N );
    }
    pi_ram_write_async(&ram, hyper_buff+(N-1)*N*sizeof(int), &tempC[N*buffer_id], (uint32_t) N* sizeof(int), pi_task_callback(&ram_write_tasks[i-1], end_of_tx, NULL));    // last transfer

    while(count != i) {
        pi_yield();
    }
#endif
#ifdef PERFORMANCE
    pi_perf_stop();
    uint32_t instr_cnt = pi_perf_read(PI_PERF_INSTR);
    uint32_t cycles_cnt = pi_perf_read(PI_PERF_CYCLES);

    printf("Number of Instructions: %d\nClock Cycles: %d\nCPI: %f%f\n", 
        instr_cnt, cycles_cnt, (float) cycles_cnt/instr_cnt);
#endif

#ifdef PRINT_MATRIX
    printf("\n\nThe result of the MatAdd is C\n");
    print_matrix(C, N);
#endif

}
