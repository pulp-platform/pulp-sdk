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
#include "stats.h"

//#define PRINT_MATRIX
#define N 128

// global variables
int A[N];
int B[N];
int tempC[N];

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
    // initialize matrix operands
    task_initMat();

#ifdef PRINT_MATRIX
    printf("\n\nThis is the Matrix A\n");
    print_matrix(A, N);
    printf("\n\nThis is the Matrix B\n");
    print_matrix(B, N);
#endif
#ifndef STATS
    //initialize performance counters
    pi_perf_conf(
        1 << PI_PERF_CYCLES | 
        1 << PI_PERF_INSTR 
    );

    // measure statistics on matrix operations
    pi_perf_reset();
    pi_perf_start();
#else
    INIT_STATS();

    PRE_START_STATS();
    START_STATS();
#endif

    for(int i=0; i<N;i++){
        task_VectProdScalar(A[i], B, tempC, N);
    }

#ifndef STATS
    pi_perf_stop();
    uint32_t instr_cnt = pi_perf_read(PI_PERF_INSTR);
    uint32_t cycles_cnt = pi_perf_read(PI_PERF_CYCLES);

    printf("Number of Instructions: %d\nClock Cycles: %d\nCPI: %f%f\n", 
        instr_cnt, cycles_cnt, (float) cycles_cnt/instr_cnt);
#else
    STOP_STATS();
#endif
}
