#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "bst_model.h"

double calculate_probability(double * probabilities, int start, int end)
{
  int i;
  double value = 0;

  for(i = start; i <= end; i++)
  {
    value += probabilities[i];
  }
  return value;
}

double calculate_entry_value(double prob1, double prob2, double total_prob)
{
  double value = prob1 + prob2 + total_prob;
  return value;
}

void set_matrices(int node_count, double probability_matrix[node_count][node_count],
  int result_matrix[node_count][node_count], double * probabilities)
{
  int i = 0;
  int j = 0;

  for(i = 0; i < node_count; i++)
  {
    for(j = 0; j < node_count; j++)
    {
      probability_matrix[i][j] = 0;
      result_matrix[i][j] = 0;
    }
  }

  for(i = 0; i < node_count; i++)
  {
    probability_matrix[i][i] = probabilities[i];
    result_matrix[i][i] = i;
  }
}

void calculate_results(int node_count, double probability_matrix[node_count][node_count],
  int result_matrix[node_count][node_count], double * probabilities)
{
  set_matrices(node_count,probability_matrix, result_matrix, probabilities);
  int k = 0;
  int i = 0;
  int j = 0;
  double left = 0;
  double right = 0;
  int chain_length = 0;
  double total_probability = 0;
  double result_value = 0;

  for(chain_length = 2; chain_length <= node_count; chain_length++)
  {
    for(i = 0; i < node_count-chain_length+1; i++)
    {
      j = i+chain_length-1;
      probability_matrix[i][j] = INT_MAX;
      total_probability = calculate_probability(probabilities,i,j);

      for(k = i; k <= j; k++)
      {

        left = ((k > i) ? probability_matrix[i][k-1] : 0);
        right = ((k < j) ? probability_matrix[k+1][j]: 0);

        result_value = calculate_entry_value(left, right, total_probability);
        if(result_value < probability_matrix[i][j])
        {
          probability_matrix[i][j] = result_value;
          result_matrix[i][j] = k;
        }
      }
    }
  }
}
