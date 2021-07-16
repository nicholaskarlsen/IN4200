#include <mpi.h>
#include <stdlib.h>
#include "MPI_double_layer_convolution.h"
#include "float2d.h"

/**
 * @brief Performs a single convolution
 *
 * NOTE: This function has been changed slightly from what is given in the exercise text. See comment below!
 *
 * @param M     : Number of rows in the input array
 * @param N     : Number of columns in the input array
 * @param input : The array to be colvolved
 * @param K     : Size of the square convolutional kernel
 * @param kernel: convolutional kernel
 * @param output: The result of the convolution
 */
void single_layer_convolution(int M, int N, float **input, int K, float **kernel, float **output)
{
    int i, j, ii, jj, out_cols = (N - K + 1);
    float temp;
    for (i = 0; i <= M - K; i++)
        for (j = 0; j <= N - K; j++)
        {
            temp = 0.0;
            for (ii = 0; ii < K; ii++)
                for (jj = 0; jj < K; jj++)
                    temp += *(input[0] + (i + ii) * N + j + jj) * kernel[ii][jj];
            // temp += input[i + ii][j + jj] * kernel[ii][jj];
            //output[i][j] = temp;
            *(output[0] + i * out_cols + j) = temp;
            /*
             * NOTE: By accessing the elements using pointer arithmetic we may use the same array
             * as both input and output, since the output will always be either larger, or the same size
             * as the input.
             * Furthermore, input[i][j] will remain untouched by the loop after output[i][j] has been
             * assigned for any pair of indices (i, j).
             * When performing two successive convolutions, we may thus allocate an array large enough
             * to hold the output of the first convolution, and then use that same array as both the
             * input and the output of the second convolution, avoiding the need to allocate arrays to
             * store the two outputs separately!
             */
        }
}

/**
 * @brief Performs two convolutions in succession in parallel on multiple threads using MPI.
 * As per the exam text, we may assume that M, N >> K1, K2, world_size. Following from these assumptions
 * this function will ONLY execute and/or yield correct results if these are upheld.
 * Furthermore; due to a particular trick i have used, the function will only work for 2 or more threads.
 * That is because the root thread has: (M-K1-K2+2)*(N-K1-K2+2) units of storage to keep the output
 * of the first convolution, which requires (M_local-K1+1)*(N-K1-1) units of storage; where M_local
 * denotes the number of rows in the input to be computed by the particular thread.
 * If running on a single thread, M=M_local and there will not be sufficient memory to perform the
 * computation. In such a case, one should in stead utilize a serial routine.
 *
 * Since this is not production code, but an exam; proper error handling is omitted, and the given
 * assumptions are assumed to be upheld. Else: expect crashes and/or buggy behaviour.
 *
 * @param M      : Number of rows in the input array
 * @param N      : Number of columns in the input array
 * @param input  : 2D, M*N array to be convolved. The array is assumed to be stored contiguously in memory!
 * @param K1     : Size of first kernel1
 * @param kernel1: 2D, K1*K1 kernel used to perform the first convolution
 * @param K2     : Size of kernel2
 * @param kernel2: 2D, K2*K2 kernel used to perform the second convolution
 * @param output : (M-K1-K2+2)*(N-K1-K2+2) array output; is also assumed to be stored contiguously in memory.
 */
void MPI_double_layer_convolution(int M, int N, float **input,
                                  int K1, float **kernel1, int K2, float **kernel2,
                                  float **output)
{
    int i, my_rank, world_size, base_work, remaining_work, M_local = 0;
    int *sendcounts = NULL, *displs_send = NULL, *recvcounts = NULL, *displs_recv = NULL;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (my_rank == 0)
    {
        // Distribute the work based on the number of rows in the final outout
        base_work = (M - K1 - K2 + 2) / world_size;
        remaining_work = (M - K1 - K2 + 2) % world_size; // if the workload isn't divisable by world_size
        // Used to scatter the input to all the workers
        sendcounts = (int *)malloc(world_size * sizeof(int));
        displs_send = (int *)malloc(world_size * sizeof(int));
        // Used to gather the output from all the workers
        recvcounts = (int *)malloc(world_size * sizeof(int));
        displs_recv = (int *)malloc(world_size * sizeof(int));

        // Send sufficient amount of rows such that each worker can perform the two convolutions
        // independently. Alternative would be to send (K1-1) to each worker and fetching
        // required rows to perform the second convolution from another node; but this would
        // require time spent to transfer memory with MPI. if M>>K2 it is likely more efficient
        // to have (K2-1) rows of duplicate work between workers in stead. But this depends on how fast
        // the memory transfer is. would be very slow on i.e a distributed cluster.
        for (i = 0; i < world_size; i++)
        {
            sendcounts[i] = base_work + K1 + K2 - 2;
            recvcounts[i] = base_work;
        }
        // Distribute the remainder out evenly
        for (i = 0; i < remaining_work; i++)
        {
            sendcounts[i]++;
            recvcounts[i]++;
        }
        // Multiply by number of columns to get actual workload in terms of array elements
        for (i = 0; i < world_size; i++)
        {
            sendcounts[i] *= N;
            recvcounts[i] *= (N - K1 - K2 + 2);
        }

        displs_send[0] = 0;
        displs_recv[0] = 0;
        for (i = 1; i < world_size; i++)
        {
            // beginning of displs_send "shifted" back by (K1 + K2 - 1) rows such that there is
            // overlap between input that is being distributed to each thread as is required to perform
            // the convolutions
            displs_send[i] = displs_send[i - 1] + (sendcounts[i - 1] - (K1 + K2 - 2) * N);
            displs_recv[i] = displs_recv[i - 1] + recvcounts[i - 1];
        }
    }

    // Tell each thread how many rows (M_local) of input it will need to compute
    MPI_Scatter(&(sendcounts[0]), 1, MPI_INT, &M_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
    M_local /= N; // recall: sendcounts = rows * cols

    // Allocate memory to store subsections input & output on non-root threads
    if (my_rank > 0)
    {
        input = init_float2d(M_local, N);
        output = init_float2d(M_local - K1 + 1, N - K1 + 1);
    }

    // It's redundant to send any data to the root worker as all the required data is already present
    if (my_rank == 0)
        sendcounts[0] = 0;

    // Scatter required subsections of input to compute subsection of final output to each thread
    MPI_Scatterv(
        &(input[0][0]), // sendbuf
        sendcounts,     // sendcount
        displs_send,    // displs
        MPI_FLOAT,      // sendtype
        &(input[0][0]), // recvbuf
        M_local * N,    // recvcount
        MPI_FLOAT,      // recvtype
        0,              // root
        MPI_COMM_WORLD);

    single_layer_convolution(M_local, N, input, K1, kernel1, output);
    // Use the output of the first one as the input of the second
    // convolution AND store the output of the second convolution in the same array.
    // For more details as to why this works; see comment in single_layer_convolution
    single_layer_convolution(M_local - K1 + 1, N - K1 + 1, output, K2, kernel2, output);

    // Gather the final output to the root thread
    MPI_Gatherv(
        &(output[0][0]),                             // sendbuf
        (M_local - K1 - K2 + 2) * (N - K1 - K2 + 2), // sendcount
        MPI_FLOAT,                                   // sendtype
        &(output[0][0]),                             // recvbuf
        recvcounts,                                  // recvcounts
        displs_recv,                                 // displs
        MPI_FLOAT,                                   // recvtype
        0,                                           // root
        MPI_COMM_WORLD);

    // Clean up memory
    if (my_rank == 0)
    {
        free(sendcounts);
        free(displs_send);
        free(recvcounts);
        free(displs_recv);
    }
    if (my_rank > 0)
    {
        free_float2d(input);
        free_float2d(output);
    }
}