/* 
 * Copyright (C) 2020 University of Bologna
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 * Authors: Enrico Tabanelli, UniBO (enrico.tabanelli3@unibo.it)
 */

#include "pmsis.h"
#include "stats.h"

#define SIZE (128)

int A[SIZE] = {0};
int B[SIZE] = {0};
int C[SIZE*SIZE] = {0};

void task_initMat()
{
    for(int i = 0; i < SIZE; i++)
    {
        A[i] = i;
        B[i] = i;
    }
}

void task_vecmul(int* vecA, int* vecB, int * matC, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            matC[j + i*dim] = vecA[j] * vecB[j];
        }
    }
}

void task_VectProdScalar(int scalarA, int* matB, int * matC, int dim)
{
    for(int i = 0; i < dim; i++)
    {
        matC[i] = scalarA * matB[i];
    }
}

int main()
{
    task_initMat();

    INIT_STATS();
    ENTER_LOOP_STATS();
    START_STATS();

    task_vecmul(A, B, C, SIZE);

    STOP_STATS();
    EXIT_LOOP_STATS();

}
