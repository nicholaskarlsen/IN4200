#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read_graph_from_file1.h"
#include "char2d.h"

/**
 * @brief Reads a text file that contains a connectivity graph, outputs the number of nodes
 * through N, allocates a 2D table of correct dimension inside the function, fills its content
 * and passes it out.
 * 
 * @param filename File containing the connectivity graph
 * @param N Number of nodes in the graph 
 * @param table2D matrix representation of the connectivity graph stored as a 2D char array.
 */
void read_graph_from_file1(char *filename, int *N, char ***table2D)
{
    FILE *fp;
    int num_edges;
    int from_node_id;
    int to_node_id;

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

    // Initialize with all entries set to 0
    *table2D = zeros_char2d(*N, *N);
    while (fscanf(fp, "%d %d\n", &from_node_id, &to_node_id) != EOF)
    {
        // Only store legal edges
        if ((from_node_id != to_node_id) && (from_node_id < *N) && (to_node_id < *N))
        {
            // Edge from i to j => Edge from j to i (i.e table2D is symmetric)
            (*table2D)[from_node_id][to_node_id] = 1;
            (*table2D)[to_node_id][from_node_id] = 1;
        }
    }

    fclose(fp);
}
