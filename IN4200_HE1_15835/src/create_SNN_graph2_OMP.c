#include <stdlib.h>
#include <omp.h>
#include "create_SNN_graph2_OMP.h"

/**
 * @brief Loops through row_ptr and col_idx lists to find number of shared neighbors by computing
 * the matrix elements SNN[i][j] = C[i][j] * (S[i][k] * S[j][k]); k = 0,1,...,N-1  where C is the
 * corresponding connectivity graph. Given that only non-zero elements are stored in CRS; the 
 * C[i][j] is made redundant. In stead; we compute SNN[i][j] by summing number of shared columns
 * between rows i and j.
 * @param N 
 * @param row_ptr 
 * @param col_idx 
 * @param SNN_val 
 */
void create_SNN_graph2_OMP(int N, int *row_ptr, int *col_idx, int **SNN_val)
{
    int i, j, a_idx, b_idx, a_stop, b_stop;
    // SNN_val will have same no. non-zero entries as col_idx, given by last entry in row_ptr.
    (*SNN_val) = (int *)malloc(row_ptr[N] * sizeof(int));
    for (i = 0; i < row_ptr[N]; i++)
        (*SNN_val)[i] = 0;

#pragma omp parallel for private(a_idx, a_stop, b_idx, b_stop) schedule(static)
    for (i = 0; i < N; i++)
    {
        for (j = row_ptr[i]; j < row_ptr[i + 1]; j++)
        {
            a_idx = row_ptr[i];
            b_idx = row_ptr[col_idx[j]];
            a_stop = row_ptr[i + 1];
            b_stop = row_ptr[col_idx[j] + 1];
            // Count number of shared columns between rows i and j.
            while ((a_idx < a_stop) && (b_idx < b_stop))
            {
                if (col_idx[a_idx] == col_idx[b_idx])
                {
                    (*SNN_val)[j]++;
                    a_idx++;
                }
                else if (col_idx[a_idx] < col_idx[b_idx])
                    a_idx++;
                else
                    b_idx++;
            }
        }
    }
}
