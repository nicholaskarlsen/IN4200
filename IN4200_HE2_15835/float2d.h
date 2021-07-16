#ifndef FLOAT2D_H
#define FLOAT2D_H
/**
 * @file : float2d.h
 * @brief: Header only library which is used to create and allocate 2-dimensional float arrays.
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Allocates a 2-dimensional N*M array of floats stored contiguously in memory.
 *
 * @param N: Number of rows in the array
 * @param M: Number of columns in the array
 * @return int**
 */
static inline float **init_float2d(int N, int M)
{
    int i;
    float **A;
    A = (float **)malloc(N * sizeof(float *));
    A[0] = (float *)malloc(N * M * sizeof(float));

    for (i = 1; i < N; i++)
        A[i] = &(A[0][M * i]);

    return A;
}

/**
 * @brief Frees up the memory of a 2-Dimensional contiguously stored array
 *
 * @param A: 2-Dimensional float array to free
 */
static inline void free_float2d(float **A)
{
    free(A[0]);
    free(A);
}

/**
 * @brief Prints the entries in a 2d array of floats in the terminal in a nicely formatted way. Used during
 * initial development/debugging.
 *
 * @param A: 2-Dimensional float array to print
 * @param N: Number of rows in the array
 * @param M: Number of columns in the array
static void print_float2d(float **A, int N, int M)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        printf("[");
        for (j = 0; j < M - 1; j++)
        {
            printf("%.2f, ", A[i][j]);
        }
        printf("%.2f]\n", A[i][M - 1]);
    }
}
*/

#endif
