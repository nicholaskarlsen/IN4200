#ifndef MPI_DOUBLE_LAYER_CONVOLUTION_H
#define MPI_DOUBLE_LAYER_CONVOLUTION_H

void single_layer_convolution(int M, int N, float **input, int K, float **kernel, float **output);
void MPI_double_layer_convolution(int M, int N, float **input, int K1, float **kernel1, int K2, float **kernel2, float **output);

#endif