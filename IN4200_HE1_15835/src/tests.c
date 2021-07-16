#include <stdlib.h>
#include <stdio.h>
#include "read_graph_from_file1.h"
#include "read_graph_from_file2.h"
#include "create_SNN_graph1.h"
#include "create_SNN_graph2.h"
#include "create_SNN_graph1_OMP.h"
#include "create_SNN_graph2_OMP.h"

/**
 * Checks if the read_graph_from_file1() function is able to reproduce a simple test case.
 * If the expectations are not met, error messages are printed to the terminal.
 * If all tests are passed, and OK message is instead printed.
*/
int test_read_graph_from_file1()
{
    printf("- %-30s", "read_graph_from_file1");
    // Keep count of how many errors there are
    int errors = 0;

    // Expected result
    int true_N = 5;
    int true_table2D[5][5] = {
        {0, 1, 1, 1, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1},
        {1, 1, 1, 0, 1},
        {0, 0, 1, 1, 0}};

    // Test result
    int N;
    char **table2D;
    read_graph_from_file1("dat/simple-graph.txt", &N, &table2D);

    if (N != true_N)
    {
        printf("\n[Test Error] N does not hold its expected value!");
        errors++;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (true_table2D[i][j] != table2D[i][j])
            {
                printf("\n[Test Error] table2D[%d][%d] does not hold its expected value: %d != %d",
                       i, j, true_table2D[i][j], table2D[i][j]);
                errors++;
            }
        }
    }

    free(table2D[0]);
    free(table2D);

    if (errors)
        printf("\nNumber of Errors: %d\n", errors);
    else
        printf(" => OK\n");

    return errors;
}

/**
 * Checks if the read_graph_from_file2() function is able to reproduce a simple test case.
 * If the expectations are not met, error messages are printed to the terminal.
 * If all tests are passed, and OK message is instead printed.
*/
int test_read_graph_from_file2()
{
    printf("- %-30s", "read_graph_from_file2");
    // Keep count of how many errors there are
    int errors = 0;

    // Expected result given in the problem set
    int true_N = 5;
    int true_row_ptr[6] = {0, 3, 6, 10, 14, 16};
    int true_col_idx[16] = {1, 2, 3, 0, 2, 3, 0, 1, 3, 4, 0, 1, 2, 4, 2, 3};

    // Test result
    int N;
    int *row_ptr;
    int *col_idx;
    read_graph_from_file2("dat/simple-graph.txt", &N, &row_ptr, &col_idx);

    if (N != true_N)
    {
        printf("\n[Test Error] N does not hold its expected value!");
        errors++;
    }

    for (int i = 0; i < N; i++)
    {
        if (row_ptr[i] != true_row_ptr[i])
        {
            printf("\n[Test Error] row_ptr[%d] does not hold its expected value: %d != %d", i, true_row_ptr[i], row_ptr[i]);
            errors++;
        }
        for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++)
        {
            if (col_idx[j] != true_col_idx[j])
            {
                printf("\n[Test Error] col_idx[%d] does not hold its expected value: %d != %d", j, true_col_idx[j], col_idx[j]);
                errors++;
            }
        }
    }

    // N+1'th value of row_ptr is not included in the above loop.
    if (row_ptr[true_N] != true_row_ptr[true_N])
    {
        printf("\n[Test Error] row_ptr[%d] 12312312 does not hold its expected value: %d != %d", true_N, true_row_ptr[true_N], row_ptr[true_N]);
        errors++;
    }

    free(col_idx);
    free(row_ptr);

    if (errors)
        printf("\nNumber of Errors: %d\n", errors);
    else
        printf(" => OK\n");

    return errors;
}

/**
 * Checks if the create_SNN_graph1() function is able to reproduce a simple test case.
 * If the expectations are not met, error messages are printed to the terminal.
 * If all tests are passed, and OK message is instead printed.
*/
int test_create_SNN_graph1()
{
    printf("- %-30s", "create_SNN_graph1");
    int errors = 0;
    // Ground truth computed by hand
    int true_SNN_table[5][5] = {
        {0, 2, 2, 2, 0},
        {2, 0, 2, 2, 0},
        {2, 2, 0, 3, 1},
        {2, 2, 3, 0, 1},
        {0, 0, 1, 1, 0}};

    int N;
    char **table2D;
    int **SNN_table;
    char *simple_graph = "dat/simple-graph.txt";

    read_graph_from_file1(simple_graph, &N, &table2D);
    create_SNN_graph1(N, table2D, &SNN_table);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (true_SNN_table[i][j] != SNN_table[i][j])
            {
                printf("\n[Test Error] SNN_table[%d][%d] does not hold its expected value: %d != %d",
                       i, j, true_SNN_table[i][j], SNN_table[i][j]);
                errors++;
            }
        }
    }

    free(table2D[0]);
    free(table2D);
    free(SNN_table[0]);
    free(SNN_table);

    if (errors)
        printf("\nNumber of Errors: %d\n", errors);
    else
        printf(" => OK\n");

    return errors;
}

/**
 * Checks if the create_SNN_graph2() function is able to reproduce a simple test case.
 * If the expectations are not met, error messages are printed to the terminal.
 * If all tests are passed, and OK message is instead printed.
*/
int test_create_SNN_graph2()
{
    printf("- %-30s", "create_SNN_graph2");
    int errors = 0;
    // Ground truth computed by hand
    int true_SNN_val[16] = {2, 2, 2, 2, 2, 2, 2, 2, 3, 1, 2, 2, 3, 1, 1, 1};

    int N;
    int *row_ptr;
    int *col_idx;
    int *SNN_val;
    char *simple_graph = "dat/simple-graph.txt";

    read_graph_from_file2(simple_graph, &N, &row_ptr, &col_idx);
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

    for (int i = 0; i < row_ptr[N]; i++)
    {
        if (true_SNN_val[i] != SNN_val[i])
        {
            printf("\n[Test Error] SNN_val[%d] does not hold its expected value: %d != %d",
                   i, true_SNN_val[i], SNN_val[i]);
            errors++;
        }
    }

    free(row_ptr);
    free(col_idx);
    free(SNN_val);

    if (errors)
        printf("\nNumber of Errors: %d\n", errors);
    else
        printf(" => OK\n");

    return errors;
}

/**
 * Performs the same test as the corresponding non _OMP function
 */
int test_create_SNN_graph1_OMP()
{
    printf("- %-30s", "create_SNN_graph1_OMP");
    int errors = 0;
    // Ground truth computed by hand
    int true_SNN_table[5][5] = {
        {0, 2, 2, 2, 0},
        {2, 0, 2, 2, 0},
        {2, 2, 0, 3, 1},
        {2, 2, 3, 0, 1},
        {0, 0, 1, 1, 0}};

    int N;
    char **table2D;
    int **SNN_table;
    char *simple_graph = "dat/simple-graph.txt";

    read_graph_from_file1(simple_graph, &N, &table2D);
    create_SNN_graph1_OMP(N, table2D, &SNN_table);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (true_SNN_table[i][j] != SNN_table[i][j])
            {
                printf("\n[Test Error] SNN_table[%d][%d] does not hold its expected value: %d != %d",
                       i, j, true_SNN_table[i][j], SNN_table[i][j]);
                errors++;
            }
        }
    }

    free(table2D[0]);
    free(table2D);
    free(SNN_table[0]);
    free(SNN_table);

    if (errors)
        printf("\nNumber of Errors: %d\n", errors);
    else
        printf(" => OK\n");

    return errors;
}

/**
 * Performs the same test as the corresponding non _OMP function
 */
int test_create_SNN_graph2_OMP()
{
    printf("- %-30s", "create_SNN_graph2_OMP");
    int errors = 0;
    // Ground truth computed by hand
    int true_SNN_val[16] = {2, 2, 2, 2, 2, 2, 2, 2, 3, 1, 2, 2, 3, 1, 1, 1};

    int N;
    int *row_ptr;
    int *col_idx;
    int *SNN_val;
    char *simple_graph = "dat/simple-graph.txt";

    read_graph_from_file2(simple_graph, &N, &row_ptr, &col_idx);
    create_SNN_graph2_OMP(N, row_ptr, col_idx, &SNN_val);

    for (int i = 0; i < row_ptr[N]; i++)
    {
        if (true_SNN_val[i] != SNN_val[i])
        {
            printf("\n[Test Error] SNN_val[%d] does not hold its expected value: %d != %d",
                   i, true_SNN_val[i], SNN_val[i]);
            errors++;
        }
    }

    free(row_ptr);
    free(col_idx);
    free(SNN_val);

    if (errors)
        printf("\nNumber of Errors: %d\n", errors);
    else
        printf(" => OK\n");

    return errors;
}

/**
 * @brief Test function that reads and computes the SNN graph for the facebook graph.
 * Lacking a ground truth, the test function checks for self-consistency between the 
 * table2D and CRS approaches.
 */
int test_self_consistency()
{
    int i, j;
    int errors = 0;
    printf("- %-30s", "self_consistency:");

    //Table 2D format
    int N_Table2D;
    char **table2D;
    int **SNN_table;

    // CRS format
    int N_CRS;
    int *row_ptr;
    int *col_idx;
    int *SNN_val;

    char *facebook_graph = "dat/facebook_combined.txt";
    //char *facebook_graph = "dat/simple-graph.txt";
    read_graph_from_file1(facebook_graph, &N_Table2D, &table2D);
    create_SNN_graph1(N_Table2D, table2D, &SNN_table);

    read_graph_from_file2(facebook_graph, &N_CRS, &row_ptr, &col_idx);
    create_SNN_graph2(N_CRS, row_ptr, col_idx, &SNN_val);

    if (N_Table2D != N_CRS)
    {
        printf("\n[Test Error] N mistmatch from read_graph_from_file1 & read_graph_from_file2: %d != %d", N_Table2D, N_CRS);
        errors++;
    }

    /**
     * Reccord a sum of all entries in both representations. This is done to ensure 100% 
     * correspondence. In the next loop; we test if all entries that are present in the CRS repr
     * matches with table2D, but there may still be non-zero elements in table2D not found in this
     * limited subset of the full array! Therefore; we can sum up all the elements in both arrays.
     * And if ALL the tests in the below loop passes in addition to the sums matching up. We can 
     * ensure that there is 100% correspondence between the two arrays!
     */
    int sum_CRS = 0;
    int sum_Table2D = 0;

    for (i = 0; i < N_CRS; i++)
    {
        for (j = row_ptr[i]; j < row_ptr[i + 1]; j++)
        {
            if (SNN_val[j] != SNN_table[i][col_idx[j]])
            {
                printf("\n[Test Error] SNN_val[%d] != SNN_table[%d][%d]", j, i, col_idx[j]);
                errors++;
            }
            sum_CRS += SNN_val[j];
        }
    }

    // Sum up ALL entries in SNN_table; even the ones not touched by the above loop.
    for (i = 0; i < N_Table2D; i++)
        for (j = 0; j < N_Table2D; j++)
            sum_Table2D += SNN_table[i][j];

    if (sum_CRS != sum_Table2D)
    {
        printf("[Test Error] Sum of all entries in SNN_val and SNN_table does not match! %d != %d", sum_CRS, sum_Table2D);
        errors++;
    }

    // If all of the above tests passes, we also get by extension that col_idx & row_ptr
    // corresponds to table2D.

    free(table2D[0]);
    free(table2D);
    free(SNN_table[0]);
    free(SNN_table);

    free(row_ptr);
    free(col_idx);
    free(SNN_val);

    if (errors)
        printf("\nNumber of Errors: %d\n", errors);
    else
        printf(" => OK\n");

    return errors;
}

/**
 * @brief performs all of the unit tests in consecutive order
 */
void run_tests()
{
    int total_errors = 0;
    printf("----------- Running tests  -----------\n");
    total_errors += test_read_graph_from_file1();
    total_errors += test_read_graph_from_file2();
    total_errors += test_create_SNN_graph1();
    total_errors += test_create_SNN_graph2();
    total_errors += test_create_SNN_graph1_OMP();
    total_errors += test_create_SNN_graph2_OMP();
    total_errors += test_self_consistency();

    if (total_errors == 0)
        printf("No errors!\n");
    else
        printf("Number of errors: %d", total_errors);
    printf("----------- Tests Complete -----------\n");
}
