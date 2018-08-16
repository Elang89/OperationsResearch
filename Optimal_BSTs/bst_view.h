#ifndef SERIES_VIEW_H
#define SERIES_VIEW_H
#include <gtk/gtk.h>
#include "file_handler.h"

typedef struct {
  int node_count;
  double * probabilities;
  char ** names;
}BSTData;

void on_window_destroy(GtkWidget *window, gpointer data);
void reset_window(GtkWidget *button, gpointer data);
int verify_node_data();
void create_node_grid();
int collect_total();
void call_load_window(GtkWidget *button, gpointer data);
void load_file_data();
void reset_on_load(FileData * file_data);
void display_results(int node_count, double probability_matrix[node_count][node_count],
  int result_matrix[node_count][node_count]);
void create_result_grid(double ** result_matrix);
void accept(GtkWidget *button, gpointer data);
void cancel(GtkWidget *button, gpointer data);
void assign_and_display_objects();

#endif
