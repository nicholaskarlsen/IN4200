#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

/**
 * @brief Print CRS formatted matrix to the terminal as a "regular" matrix in a memory efficient way
 * Assumes that all rows has at least one entry (i.e all nodes have at least one connection).
 * This was mostly used during initial development of the CRS functionalities.
 * 
 * @param N 
 * @param row_ptr 
 * @param col_idx 
 */
void print_CRS(int N, int *row_ptr, int *col_idx, int *val)
{
    int i, j, k, c;

    for (i = 0; i < N; i++)
    {
        printf("Row %-4d[", i);
        c = 0; // Keep track of how many columns have been printed thus far
        for (j = row_ptr[i]; j < row_ptr[i + 1]; j++)
        {
            // Check if any zeroes need to be printed before the next non-zero column
            if (c < col_idx[j])
            {
                for (k = 0; k < (col_idx[j] - c); k++)
                    printf("0 ");
                c += (col_idx[j] - c); // Increment column counter by how many zeroes were added
            }
            printf("%d ", val[j]);
            c++;
        }
        // Check if there should be any zeros at the end of the row
        if (c < N)
        {
            for (k = 0; k < N - c; k++)
                printf("0 ");
        }
        printf("]\n");
    }
}