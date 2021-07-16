#include <stdlib.h>
#include <stdio.h>
#include "check_node.h"

/**
 * @brief Checks if a given node_id has neighbors which are not yet part of the cluster, that
 * should be added to the cluster
 * 
 * @param in_cluster Array that keeps track of the nodes currently added to the cluster
 * @param checked Array that keeps track of the nodes that have been checked by this function
 * @param node_id Id of node currently being checked
 * @param tau threshold value of adding to the cluster
 * @param N Total number of nodes in the graph
 * @param row_ptr 
 * @param col_idx 
 * @param SNN_val 
 */
void find_nodes(char **in_cluster, char **checked, int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val)
{
    (*checked)[node_id] = 1;
    for (int i = row_ptr[node_id]; i < row_ptr[node_id + 1]; i++)
    {
        if ((tau <= SNN_val[i]) && !(*in_cluster)[col_idx[i]])
            (*in_cluster)[col_idx[i]] = 1;
    }
}

/**
 * @brief Checks if a node forms a cluster; and prints out all its constituents in the terminal
 * 
 * @param node_id 
 * @param tau 
 * @param N 
 * @param row_ptr 
 * @param col_idx 
 * @param SNN_val 
 */
void check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val)
{
    int i;
    // Arrays to keep track of nodes which are confirmed to be in the cluster & which have
    // already been checked for additional neighbors to be in the cluster
    char *in_cluster = (char *)malloc(N * sizeof(char));
    char *checked = (char *)malloc(N * sizeof(char));
    printf("Checking Node %d: ", node_id);

    // Initialize to zero
    for (i = 0; i < N; i++)
    {
        in_cluster[i] = 0;
        checked[i] = 0;
    }

    in_cluster[node_id] = 1;
    int next_node = node_id;
    do
    {
        find_nodes(&in_cluster, &checked, next_node, tau, N, row_ptr, col_idx, SNN_val);
        // Look for a node which has not yet been checked
        i = 0;
        next_node = -1; // since all valid node ids are non-negative.
        while ((i < N) && (next_node == -1))
        {
            if (in_cluster[i] && !checked[i])
            {
                next_node = i;
            }
            i++;
        }
    } while (next_node != -1);

    // "...the function should print out the other nodes inside the cluster..."
    // => Remove node_id from the in_cluster list now that its no longer required for the algorithm.
    in_cluster[node_id] = 0;

    printf("-> [ ");
    for (i = 0; i < N; i++)
    {
        if (in_cluster[i])
            printf("%d ", i);
    }
    printf("]\n");

    free(in_cluster);
    free(checked);
}