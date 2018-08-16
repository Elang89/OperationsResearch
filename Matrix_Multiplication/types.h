#ifndef TYPES_H
#define TYPES_H


typedef struct {
  int matrix_count;
  int dimensions[30];
  int cost_matrix[30][30];
  int result_matrix[30][30];
} MatrixData;

typedef struct {
  int matrix_count;
  int status;
  int dimensions[30];
} FileData;

#endif
