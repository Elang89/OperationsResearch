#ifndef STRUCTS_H
#define STRUCTS_H
#include "graph.h"

typedef struct WidgetGroup
{
  GtkWidget *btn_accept_size;
  GtkWidget *btn_cancel_size;
  GtkWidget *btn_open_file_window;
  GtkWidget *btn_run_floyd;
  GtkWidget *graph_size_field;;
  GtkWidget *grid_matrix;

}WidgetGroup;

typedef struct TableWindowWidgets
{
  GtkWidget *p_grid;
  GtkWidget *grid_tables[11];
  GtkWidget *btn_next_table;
  GtkWidget *btn_shortest_paths;
  WidgetGroup *parent_object;
  Graph *graph;
  char names[20][20];
  int matrix[20][20];
  int matrix_init;
  int current_table_number;
}TableWindowWidgets;

typedef struct RouteWidgets{
  TableWindowWidgets *parent_object;
  GtkWidget *btn_calc_route;
  GtkWidget *source_combo;
  GtkWidget *route_text;
  GtkWidget *dest_combo;
}RouteWidgets;

#endif
