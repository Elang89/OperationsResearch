#ifndef BST_MODEL_H
#define BST_MODEL_H


void calculate_results(int matrix_count, int cost_matrix[matrix_count][matrix_count],
  int result_matrix[matrix_count][matrix_count], int * dimensions);
void set_matrices(int matrix_count, int cost_matrix[matrix_count][matrix_count],
  int result_matrix[matrix_count][matrix_count], int * dimensions);
int calculate_dimension_cost(int * dimensions, int i, int k, int j);
int calculate_entry_value(int value1, int value2, int total);

#endif
