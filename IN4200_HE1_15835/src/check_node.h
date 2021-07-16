#pragma once

void find_nodes(char **in_cluster, char **checked, int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val);
void check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val);