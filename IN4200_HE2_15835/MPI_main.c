#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "MPI_double_layer_convolution.h"
#include "float2d.h"

int main(int nargs, char **args)
{
    int M = 0, N = 0, K1 = 0, K2 = 0, my_rank, i, j;
    float **input = NULL, **output = NULL, **kernel1 = NULL, **kernel2 = NULL;

    MPI_Init(NULL, NULL); // args treated on root thread & distributed later
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
    {
        // read from command line the values of M, N, and K
        // allocate 2D array ’input’ with M rows and N columns
        // allocate 2D array ’output’ with M-K+1 rows and N-K+1 columns
        // allocate the convolutional kernel with K rows and K columns
        // fill 2D array ’input’ with some values// fill kernel with some values
        // ....
        if (nargs == 5)
        {
            M = atoi(args[1]);
            N = atoi(args[2]);
            K1 = atoi(args[3]);
            K2 = atoi(args[4]);
        }
        else
        {
            // Fall back on a sane set of defaults
            printf("M, N, K1, K2 not provided. Falling back to M=N=1000, K1=K2=3\n");
            M = 1000;
            N = 1000;
            K1 = 3;
            K2 = 3;
        }

        input = init_float2d(M, N);
        output = init_float2d(M - K1 - K2 + 2, N - K1 - K2 + 2);
        kernel1 = init_float2d(K1, K1);
        kernel2 = init_float2d(K2, K2);

        // Fill input & kernels with random numbers in the interval [0, 1000]
        for (i = 0; i < M; i++)
            for (j = 0; j < N; j++)
                input[i][j] = (float)rand() / (float)(RAND_MAX / 1000);

        for (i = 0; i < K1; i++)
            for (j = 0; j < K1; j++)
                kernel1[i][j] = (float)rand() / (float)(RAND_MAX / 1000);

        for (i = 0; i < K2; i++)
            for (j = 0; j < K2; j++)
                kernel2[i][j] = (float)rand() / (float)(RAND_MAX / 100);
    }
    // process 0 broadcasts values of M, N, K to all the other processes
    // ...
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K2, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank > 0)
    {
        // allocated the convolutional kernel with K rows and K columns
        // ...
        kernel1 = init_float2d(K1, K1);
        kernel2 = init_float2d(K2, K2);
    }
    // process 0 broadcasts the content of kernel to all the other processes
    // ...
    MPI_Bcast(&(kernel1[0][0]), K1 * K1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&(kernel2[0][0]), K2 * K2, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // parallel computatin of a single-layer convolution
    MPI_double_layer_convolution(M, N, input, K1, kernel1, K2, kernel2, output);

    if (my_rank == 0)
    {
        // For example, compare the content of array ’output’ with that is
        // produced by the sequential function single_layer_convolution3
        // ...

        // perform single-threaded convolutions in a simple, unoptimized way to check correctness of MPI implementation
        float **output_st_1 = init_float2d(M - K1 + 1, N - K1 + 1);           // after first convolution
        float **output_st_2 = init_float2d(M - K1 - K2 + 2, N - K1 - K2 + 2); // after second convolution
        single_layer_convolution(M, N, input, K1, kernel1, output_st_1);
        single_layer_convolution(M - K1 + 1, N - K1 + 1, output_st_1, K2, kernel2, output_st_2);

        // Compare and count differences!
        int errors = 0;
        for (int i = 0; i < M - K1 - K2 + 2; i++)
            for (int j = 0; j < N - K1 - K2 + 2; j++)
                if (output[i][j] != output_st_2[i][j])
                {
                    printf("output[%d][%d] = %.2f != %.2f = output_st_2\n",
                           i, j, output[i][j], output_st_2[i][j]);
                    errors++;
                }
        printf("Total number of differences between MPI and single-thread convolutions: %d\n", errors);

        free_float2d(output_st_1);
        free_float2d(output_st_2);
        free_float2d(output);
        free_float2d(input);
    }

    free_float2d(kernel1);
    free_float2d(kernel2);

    MPI_Finalize();

    return 0;
}
