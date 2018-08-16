#ifndef BST_MODEL_H
#define BST_MODEL_H


void calculate_results(int node_count, double probability_matrix[node_count][node_count],
  int result_matrix[node_count][node_count], double * probabilities);
void set_matrices(int node_count, double probability_matrix[node_count][node_count],
  int result_matrix[node_count][node_count], double * probabilities);
double calculate_probability(double * probabilities, int start, int end);
double calculate_entry_value(double prob1, double prob2, double total_prob);

#endif
