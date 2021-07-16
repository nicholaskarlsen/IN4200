#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Compares two integers; used in qsort call in read_graph_from_file2 to sort *col_idx
 * 
 * @param a Integer to compare
 * @param b Integer to compare
 * @return int
 */
int compare_int(const void *a, const void *b)
{
    int *x = (int *)a;
    int *y = (int *)b;
    return *x - *y;
}

/**
 * @brief reads a connectivity graph input file and stores it in the CRS format.
 * 
 * @param filename 
 * @param N 
 * @param row_ptr 
 * @param col_idx 
 */
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx)
{
    FILE *fp;
    int num_edges;
    int from_node_id;
    int to_node_id;
    int i, c;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Unable to read file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    // Expect: first two lines to contain documentation -> skip lines
    fscanf(fp, "%*[^\n]\n");
    fscanf(fp, "%*[^\n]\n");
    // Expect: Line 3 "# Nodes: {N} Edges: {edges}"
    fscanf(fp, "%*s %*s %d %*s %d \n", N, &num_edges);
    // Expect: Line 4 "# FromNodeId ToNodeId" -> skip line
    fscanf(fp, "%*[^\n]\n");

    (*row_ptr) = (int *)malloc((*N + 1) * sizeof(int));
    // Temporary storage for all the connections
    int *from_idx = (int *)malloc(2 * num_edges * sizeof(int));
    int *to_idx = (int *)malloc(2 * num_edges * sizeof(int));

    for (i = 0; i < (*N + 1); i++)
        (*row_ptr)[i] = 0;

    c = 0; // Keep track of number of valid connections
    while (fscanf(fp, "%d %d\n", &from_node_id, &to_node_id) != EOF)
    {
        // Only store the legal edges
        if ((from_node_id != to_node_id) && (from_node_id < *N) && (to_node_id < *N))
        {
            (*row_ptr)[to_node_id + 1]++;
            from_idx[c] = from_node_id;
            to_idx[c] = to_node_id;
            c++;
            // add corresponding "symmetric" entries as well
            (*row_ptr)[from_node_id + 1]++;
            from_idx[c] = to_node_id;
            to_idx[c] = from_node_id;
            c++;
        }
    }
    fclose(fp);

    for (i = 1; i < *N + 1; i++)
        (*row_ptr)[i] += (*row_ptr)[i - 1];

    (*col_idx) = (int *)malloc(c * sizeof(int));

    // Keep track of how many places in each row has already been filled
    int *N_filled = (int *)malloc(*N * sizeof(int));
    for (i = 0; i < *N; i++)
        N_filled[i] = 0;

    for (i = 0; i < c; i++)
    {
        (*col_idx)[(*row_ptr)[to_idx[i]] + N_filled[to_idx[i]]] = from_idx[i];
        N_filled[to_idx[i]]++;
    }

    // sort sub-arrays in col_idx corresponding to each row such that the columns appear
    // in ascending order
    for (i = 0; i < *N - 1; i++)
    {
        qsort(
            (*col_idx + (*row_ptr)[i]),        // => Entry corresponding to first entry in row
            (*row_ptr)[i + 1] - (*row_ptr)[i], // => Number of elements in the row
            sizeof(int),
            compare_int);
    }

    free(N_filled);
    free(from_idx);
    free(to_idx);
}
