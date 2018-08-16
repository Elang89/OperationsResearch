#include <gtk/gtk.h>
#include "hashmap.h"
#ifndef RESULTS_H
#define RESULTS_H


void show_results(GtkWidget *parent, int **adj_matrix,
  int product_viability, int plan_duration);
void calculate_paths(int **paths,
  int product_viability, int plan_duration);
  void get_path(int position, int dest, int **adj_matrix,
    int * node_visits, int * path, int path_index);
void display_paths(HashMap* hashmap, int path_count);
void on_results_destroy(GtkWidget* window, gpointer data);

#endif
