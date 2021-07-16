/**
 * @file char2d.c
 * @brief Functions used to create and allocate 2-dimensional char arrays.
 * @version 0.1
 */

#include <stdlib.h>
#include <stdio.h>
#include "char2d.h"

/**
 * @brief Allocates a 2-dimensional N*M array of characters stored contiguously in memory.
 * 
 * @param N Number of rows in the array
 * @param M Number of columns in the array
 * @return char** 
 */
char **init_char2d(int N, int M)
{
    int i;
    char **A;
    A = (char **)malloc(N * sizeof(char *));
    A[0] = (char *)malloc(N * M * sizeof(char));

    for (i = 1; i < N; i++)
        A[i] = &(A[0][M * i]);

    return A;
}

/**
 * @brief Allocates a 2-dimensional N*M array of characters stored contiguously in memory and 
 *  initializes all its entries to 0.
 * 
 * @param N Number of rows in the array
 * @param M Number of columns in the array 
 * @return char** 
 */
char **zeros_char2d(int N, int M)
{
    int i, j;
    char **A = init_char2d(N, M);

    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            A[i][j] = 0;

    return A;
}

/**
 * @brief Prints the entries in a 2d array of characters in the terminal in a nicely formatted way.
 * 
 * @param A 2-Dimensional char array to print
 * @param N Number of rows in the array
 * @param M Number of columns in the array
 */
void print_char2d(char **A, int N, int M)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        printf("Row %-4d[", i);
        for (j = 0; j < M - 1; j++)
        {
            printf("%d, ", A[i][j]);
        }
        printf("%d]\n", A[i][M - 1]);
    }
}