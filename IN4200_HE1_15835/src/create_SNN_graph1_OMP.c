#include <stdlib.h>
#include <omp.h>
#include "create_SNN_graph1_OMP.h"
#include "int2d.h"

/**
 * @brief Create a SNN graph represented as 2D table for a given connectivity graph with N nodes.
 * 
 * @param N Number of nodes present in the graph
 * @param table2D Connectivity graph
 * @param SNN_table Un-initialized pointer to the SNN graph
 */
void create_SNN_graph1_OMP(int N, char **table2D, int ***SNN_table)
{
    int i, j, k;
    *SNN_table = zeros_int2d(N, N);

#pragma omp parallel for schedule(dynamic)
    for (i = 0; i < N; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            if (table2D[i][j]) // Nonzero for all connected nodes
            {
                for (k = 0; k < N; k++)
                {
                    // Symmetry => A[k][j] = A[j][k]. Latter is faster because row-major storage
                    (*SNN_table)[i][j] += table2D[i][k] * table2D[j][k];
                }
                // Set SNN_ji to SNN_ij manually, since we're only iterating above diagonal.
                (*SNN_table)[j][i] = (*SNN_table)[i][j];
            }
        }
    }
}
