#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "knapsack_backend.h"


KnapsackData* knapsack_data_new(char** names, int* values, int* weights, int* amounts, int capacity, int object_number)
{
  int i;
  int j;
  KnapsackData *knapsack_data = malloc(sizeof(KnapsackData));

  knapsack_data->capacity = capacity;
  knapsack_data->object_number = object_number;
  knapsack_data->object_names = names;
  knapsack_data->object_values = values;
  knapsack_data->object_weights = weights;
  knapsack_data->object_amounts = amounts;
  knapsack_data->knapsack_final_weight = 0;
  knapsack_data->taken = 0;

  knapsack_data->color_matrix = (int**)malloc(object_number * sizeof(int*) + 1);
  knapsack_data->data_matrix = (int**)malloc(object_number * sizeof(int*) + 1);
  knapsack_data->objects_taken = (int*)malloc(object_number * sizeof(int*) + 1);
  knapsack_data->objects_taken_names = (char**)malloc(object_number * sizeof(char*) + 1);

  for(i = 0; i <= object_number; i++)
  {
    knapsack_data->color_matrix[i] = (int*)malloc(sizeof(int) * capacity + 1);
    knapsack_data->data_matrix[i] = (int*)malloc(sizeof(int) * capacity + 1);
    knapsack_data->objects_taken_names[i] = malloc(sizeof(char) * 20);
    knapsack_data->objects_taken[i] = 0;
  }

  for(i = 0; i <= object_number; i++)
  {
    for(j = 0; j <= capacity; j++)
    {
      knapsack_data->color_matrix[i][j] = 0;
      knapsack_data->data_matrix[i][j] = 0;
    }
  }

  calculate_optimal_solution(knapsack_data);
  return knapsack_data;
}


void calculate_optimal_solution(KnapsackData* knapsack_data)
{
  int object_value;
  int final_weight = 0;
  int final_value = 0;
  int object;
  int target_weight;
  int weight;
  int amount = 0;

  for(weight = 0; weight < knapsack_data->capacity; weight++)
  {
      knapsack_data->data_matrix[0][weight] = 0;
  }

  for(object = 0; object < knapsack_data->object_number; object++)
  {
    knapsack_data->data_matrix[object][0] = 0;
  }

  for(object = 1; object <= knapsack_data->object_number; object++)
  {
    for(weight = 0; weight <= knapsack_data->capacity; weight++)
    {
      for(amount = 0; weight - amount * knapsack_data->object_weights[object] >= 0 &&
        amount <= knapsack_data->object_amounts[object]; amount++)
      {
        final_value = amount * knapsack_data->object_values[object];
        final_weight = amount * knapsack_data->object_weights[object];
      }

      if(knapsack_data->object_weights[object] <= final_weight)
      {
        object_value = knapsack_data->data_matrix[object-1][weight-final_weight];
      }
      else
      {
        object_value = 0;
      }

      if((knapsack_data->object_weights[object] <= final_weight) &&
      object_value+final_value > knapsack_data->data_matrix[object-1][weight])
      {
        knapsack_data->data_matrix[object][weight] = object_value + final_value;
        knapsack_data->color_matrix[object][weight] = amount - 1;
      }
      else
      {
        knapsack_data->data_matrix[object][weight] = knapsack_data->data_matrix[object-1][weight];
        knapsack_data->color_matrix[object][weight] = 0;
      }
    }
  }

  target_weight = weight - 1;
  for(object = knapsack_data->object_number; object >= 1; object--)
  {
    if(knapsack_data->color_matrix[object][target_weight] >  0)
    {
      knapsack_data->objects_taken[object] = knapsack_data->color_matrix[object][target_weight];
      strcpy(knapsack_data->objects_taken_names[knapsack_data->taken],knapsack_data->object_names[object]);
      knapsack_data->knapsack_final_weight += knapsack_data->object_weights[object] * knapsack_data->color_matrix[object][target_weight];
      target_weight -= knapsack_data->object_weights[object] * knapsack_data->color_matrix[object][target_weight];
      knapsack_data->taken++;
    }
  }
}

void knapsack_data_destroy(KnapsackData* knapsack_data)
{
  int i;

  for(i = 0; i < knapsack_data->object_number; i++)
  {
      free(knapsack_data->color_matrix[i]);
      free(knapsack_data->data_matrix[i]);
      free(knapsack_data->object_names[i]);
  }
  free(knapsack_data->object_values);
  free(knapsack_data->object_weights);
  free(knapsack_data->object_amounts);

  free(knapsack_data->data_matrix);
  free(knapsack_data->color_matrix);
}
