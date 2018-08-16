#ifndef KNAPSACK_BACKEND_H
#define KNAPSACK_BACKEND_H


typedef struct KnapsackData{
  int capacity;
  int object_number;
  int taken;
  int knapsack_final_weight;
  char **object_names;
  char **objects_taken_names;
  int * object_values;
  int * object_weights;
  int * object_amounts;
  int * objects_taken;
  int **color_matrix;
  int **data_matrix;
}KnapsackData;

KnapsackData* knapsack_data_new(char** names, int* costs, int* weights, int* amounts, int knapsack_capacity, int object_number);
void calculate_optimal_solution(KnapsackData* knapsack_data);
void find_objects_taken(KnapsackData* knapsack_data);
void knapsack_data_destroy(KnapsackData* knapsack_data);

#endif
