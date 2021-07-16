#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "create_SNN_graph1.h"
#include "create_SNN_graph2.h"
#include "create_SNN_graph1_OMP.h"
#include "create_SNN_graph2_OMP.h"
#include "read_graph_from_file1.h"
#include "read_graph_from_file2.h"
#include "check_node.h"
#include "char2d.h"
#include "int2d.h"
#include "tests.h"
#include "utils.h"

#ifndef MAX_THREADS
#define MAX_THREADS 8
#endif

#ifndef NUM_TRIALS
#define NUM_TRIALS 10
#endif

void run_table_format()
{
    clock_t t_start, t_stop;
    double read_time = 0;
    double SNN_time = 0;

    for (int trial = 0; trial < NUM_TRIALS; trial++)
    {
        int N;
        char **table2D;
        //char *graph_file = "dat/simple-graph.txt";
        char *graph_file = "dat/facebook_combined.txt";

        t_start = clock();
        read_graph_from_file1(graph_file, &N, &table2D);
        t_stop = clock();
        read_time += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

        int **SNN_table;
        t_start = clock();
        create_SNN_graph1(N, table2D, &SNN_table);
        t_stop = clock();
        SNN_time += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

        free(SNN_table[0]);
        free(SNN_table);
        free(table2D[0]);
        free(table2D);
    }

    read_time /= NUM_TRIALS;
    SNN_time /= NUM_TRIALS;
    printf("read_graph_from_file1() time = %.1f\n", read_time);
    printf("create_SNN_graph1() time = %.1f\n", SNN_time);
}

void run_table_format_OMP()
{
    clock_t t_start, t_stop;
    double read_time = 0;
    double SNN_time[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++)
    {
        SNN_time[i] = 0;
    }

    for (int trial = 0; trial < NUM_TRIALS; trial++)
    {
        int N;
        char **table2D;
        //char *graph_file = "dat/simple-graph.txt";
        char *graph_file = "dat/facebook_combined.txt";

        t_start = clock();
        read_graph_from_file1(graph_file, &N, &table2D);
        t_stop = clock();
        read_time += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

        for (int N_threads = 1; N_threads <= MAX_THREADS; N_threads++)
        {
            omp_set_num_threads(N_threads);
            int **SNN_table;
            t_start = clock();
            create_SNN_graph1_OMP(N, table2D, &SNN_table);
            t_stop = clock();
            SNN_time[N_threads - 1] += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

            free(SNN_table[0]);
            free(SNN_table);
        }
        free(table2D[0]);
        free(table2D);
    }

    read_time /= NUM_TRIALS;
    printf("read_graph_from_file1() time = %.1f\n", read_time);

    for (int N_threads = 0; N_threads < MAX_THREADS; N_threads++)
    {
        SNN_time[N_threads] /= NUM_TRIALS;
        printf("[Num Threads = %d] create_SNN_graph1() time = %.1f\n", N_threads + 1, SNN_time[N_threads]);
    }
}

void run_CRS_format()
{
    clock_t t_start, t_stop;
    double read_time = 0;
    double SNN_time = 0;

    for (int trial = 0; trial < NUM_TRIALS; trial++)
    {
        int N;
        int *row_ptr;
        int *col_idx;
        int *SNN_val;
        char *graph_file = "dat/facebook_combined.txt";

        t_start = clock();
        read_graph_from_file2(graph_file, &N, &row_ptr, &col_idx);
        t_stop = clock();
        read_time += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

        t_start = clock();
        create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
        t_stop = clock();
        SNN_time += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

        // Free memory
        free(row_ptr);
        free(col_idx);
        free(SNN_val);
    }

    read_time /= NUM_TRIALS;
    SNN_time /= NUM_TRIALS;
    printf("read_graph_from_file2() time = %.1f\n", read_time);
    printf("create_SNN_graph2() time = %.1f\n", SNN_time);
}

void run_CRS_format_OMP()
{
    clock_t t_start, t_stop;
    double read_time = 0;
    double SNN_time[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++)
    {
        SNN_time[i] = 0;
    }

    for (int trial = 0; trial < NUM_TRIALS; trial++)
    {
        int N;
        int *row_ptr;
        int *col_idx;
        int *SNN_val;
        char *graph_file = "dat/facebook_combined.txt";

        t_start = clock();
        read_graph_from_file2(graph_file, &N, &row_ptr, &col_idx);
        t_stop = clock();
        read_time += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

        for (int N_threads = 1; N_threads <= MAX_THREADS; N_threads++)
        {
            omp_set_num_threads(N_threads);
            t_start = clock();
            create_SNN_graph2_OMP(N, row_ptr, col_idx, &SNN_val);
            t_stop = clock();
            SNN_time[N_threads - 1] += 1000.0 * (t_stop - t_start) / CLOCKS_PER_SEC; // in milliseconds

            free(SNN_val);
        }
        free(row_ptr);
        free(col_idx);
    }

    read_time /= NUM_TRIALS;
    printf("read_graph_from_file2() time = %.1f\n", read_time);

    for (int N_threads = 0; N_threads < MAX_THREADS; N_threads++)
    {
        SNN_time[N_threads] /= NUM_TRIALS;
        printf("[Num Threads = %d] create_SNN_graph2_OMP() time = %.1f\n", N_threads + 1, SNN_time[N_threads]);
    }
}

// Checks if check_node yields reasonable results
void run_check_node()
{
    int N;
    int *row_ptr;
    int *col_idx;
    int *SNN_val;
    char *graph_file = "dat/simple-graph.txt";
    //char *graph_file = "dat/facebook_combined.txt";

    read_graph_from_file2(graph_file, &N, &row_ptr, &col_idx);
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

    printf("\nLooking for clusters in %s:\n", graph_file);
    for (int tau = 1; tau < 4; tau++)
    {
        printf("tau = %d\n", tau);
        for (int id = 0; id < 5; id++)
        {
        check_node(id, tau, N, row_ptr, col_idx, SNN_val);

        }
    }
    // Free memory
    free(row_ptr);
    free(col_idx);
    free(SNN_val);
}

int main()
{
    // Run the unit tests
    run_tests();

    // Run the benchmarks
    printf("Running Benchmarks:\n");
    printf("2D Table representation\n");
    printf("- Serial:\n");
    run_table_format();
    printf("- Parallel:\n");
    run_table_format_OMP();
    printf("\nCRS representation\n");
    printf("- Serial:\n");
    run_CRS_format();
    printf("- Parallel:\n");
    run_CRS_format_OMP();

    // test the check_node function
    run_check_node();

    return 0;
}
