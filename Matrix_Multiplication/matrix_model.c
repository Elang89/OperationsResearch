#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "matrix_model.h"

int calculate_dimension_cost(int * dimensions, int i, int k, int j)
{
  int value = 0;

  value = dimensions[i] *  dimensions[k] * dimensions[j];

  return value;
}

int calculate_entry_value(int value1, int value2, int total)
{
  int value = value1 + value2 + total;
  return value;
}

void set_matrices(int matrix_count, int cost_matrix[matrix_count][matrix_count],
  int result_matrix[matrix_count][matrix_count], int * dimensions)
{
  int i = 0;
  int j = 0;

  for(i = 0; i < matrix_count; i++)
  {
    for(j = 0; j < matrix_count; j++)
    {
      cost_matrix[i][j] = 0;
      result_matrix[i][j] = 0;
    }
  }

  for (i = 0; i < matrix_count; i++)
        cost_matrix[i][i] = 0;
}

void calculate_results(int matrix_count, int cost_matrix[matrix_count][matrix_count],
  int result_matrix[matrix_count][matrix_count], int * dimensions)
{
  set_matrices(matrix_count,cost_matrix, result_matrix, dimensions);
  int k = 0;
  int i = 0;
  int j = 0;
  int left = 0;
  int right = 0;
  int chain_length = 0;
  int total = 0;
  int result_value = 0;

  for(chain_length = 2; chain_length <= matrix_count; chain_length++)
  {
    for(i = 0; i < matrix_count-chain_length+1; i++)
    {
      j = i+chain_length-1;
      cost_matrix[i][j] = INT_MAX;

      for(k = i; k <= j-1; k++)
      {
        total = calculate_dimension_cost(dimensions,i,k+1,j+1);
        left = cost_matrix[i][k];
        right = cost_matrix[k+1][j];
        result_value = calculate_entry_value(left, right, total);

        if(result_value < cost_matrix[i][j])
        {
          cost_matrix[i][j] = result_value;
          result_matrix[i][j] = k;
        }
      }
    }
  }
}
