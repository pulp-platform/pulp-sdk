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
#define CHECK_RESULTS
#define N_WORD 500
#define N_BYTE N_WORD*sizeof(int)

// global variables
int A[N_WORD];
int B[N_WORD];
#ifndef DOUBLE_BUFFERING
int tempC[N_WORD];
#else
int tempC[2*N_WORD];
static struct pi_task ram_write_tasks[N_WORD];
static int ram_returns = 0;
// Callback for asynchronous ram write
static void end_of_tx(void *arg)
{
  //printf("End of %d TX \n", count);
  ram_returns++;
}
#endif
char memC[N_BYTE];
static uint32_t hyper_buff; // result on L3

// global struct define
static struct pi_device ram;
static struct pi_hyperram_conf conf;


// matrix functions
void task_initMat()
{
    for(int i=0;i<N_WORD;i++){
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

    if (pi_ram_alloc(&ram, &hyper_buff, N_BYTE*N_BYTE))
    {
        printf("Ram malloc failed !\n");
        pmsis_exit(-4);
    }

    // initialize matrix operands
    task_initMat();

#ifdef PRINT_MATRIX
    printf("\n\nThis is the Matrix A\n");
    print_matrix(A, N_WORD);
    printf("\n\nThis is the Matrix B\n");
    print_matrix(B, N_WORD);
#endif
    //initialize performance counters
    pi_perf_conf(
        1 << PI_PERF_CYCLES | 
        1 << PI_PERF_INSTR 
    );

    // measure statistics on matrix operations
    pi_perf_reset();
    pi_perf_start();

#ifndef DOUBLE_BUFFERING
    for(int i=0; i<N_WORD;i++){
        task_VectProdScalar(A[i], B, tempC, N_WORD);
        pi_ram_write(&ram, hyper_buff+i*N_BYTE, tempC, (uint32_t) N_BYTE);
    }
#else
    int i_curr=1;
    int i_prev=0;
    int buffer_id;
    task_VectProdScalar(A[0], B, tempC, N_WORD);
    for(i_curr; i_curr<N_WORD;i_curr++){
        buffer_id = i_curr & 0x1;
        pi_ram_write_async(&ram, hyper_buff+i_prev*N_BYTE, &tempC[N_WORD*(1-buffer_id)], (uint32_t) N_BYTE, pi_task_callback(&ram_write_tasks[i_prev], end_of_tx, NULL));
        task_VectProdScalar(A[i_curr], B, &tempC[N_WORD*buffer_id], N_WORD);
        i_prev++;
    }
    pi_ram_write_async(&ram, hyper_buff+(N_WORD-1)*N_BYTE, &tempC[N_WORD*buffer_id], (uint32_t) N_BYTE, pi_task_callback(&ram_write_tasks[i_prev], end_of_tx, NULL));    // last transfer

    while(ram_returns != i_curr) {
        pi_yield();
    }
#endif

    pi_perf_stop();
    uint32_t instr_cnt = pi_perf_read(PI_PERF_INSTR);
    uint32_t cycles_cnt = pi_perf_read(PI_PERF_CYCLES);

    printf("Number of Instructions: %d\nClock Cycles: %d\nCPI: %f%f\n", 
        instr_cnt, cycles_cnt, (float) cycles_cnt/instr_cnt);

#ifdef CHECK_RESULTS
    for(int i=0; i<N_WORD;i++){
        pi_ram_read(&ram, hyper_buff+i*N_BYTE, memC, (uint32_t) N_BYTE);
        task_VectProdScalar(A[i], B, tempC, N_WORD);
        for(int j=0; j<N_WORD;j++){
            if(tempC[j] != *((int*) memC+j)){
                printf("Error, row: %d, index: %d, expected: 0x%x, read: 0x%x\n", i, j, tempC[j], *((int*) memC+j));
                pmsis_exit(-5);
            }
        }
    }

    printf("Test success\n");
#endif

    pi_ram_free(&ram, hyper_buff, N_BYTE*N_BYTE);
    pi_ram_close(&ram);
}
