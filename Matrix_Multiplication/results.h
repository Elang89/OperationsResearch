#ifndef RESULTS_H
#define RESULTS_H
#include <gtk/gtk.h>

void form_result_string(int matrix_count, int start, int end,
  int result_matrix[matrix_count][matrix_count]);
void destroy_results();
void display_result_window(GtkWidget * window, int matrix_count,
  int result_matrix[matrix_count][matrix_count], int final_result);


#endif
