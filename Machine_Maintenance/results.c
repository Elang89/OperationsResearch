#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "results.h"
#include "stack.h"


GtkWidget* result_window;
GtkWidget* scrolledwindow_paths;
GtkWidget* viewport_paths;
GtkWidget* grid_paths;
GtkWidget* weight_text;
int widget_counter = 1;

void on_results_destroy(GtkWidget* window, gpointer data)
{
  GtkWidget* parent = (GtkWidget*)gtk_window_get_transient_for(GTK_WINDOW(window));
  gtk_widget_set_sensitive(parent, TRUE);
  gtk_widget_destroy(result_window);
}

void add_child_to_grid(int * path, int length)
{
  int i;
  char str[200];
  char final_str[2000];
  GtkWidget *path_box = gtk_entry_new();
  strcpy(final_str,"");

  for(i = 0; i < length; i++)
  {
    sprintf(str, "%d", path[i]);
    strcat(final_str, str);
    if(i < length - 1){
      strcat(final_str, "->");
    }
    str[0] = '\0';
  }

  gtk_entry_set_text(GTK_ENTRY(path_box),final_str);
  g_object_set(path_box, "editable", FALSE, NULL);
  gtk_grid_attach(GTK_GRID(grid_paths),path_box,0,widget_counter,1,1);
  final_str[0] = '\0';
  widget_counter++;
}

void get_path(int position, int dest, int **adj_matrix,
  int * node_visits, int * path, int path_index)
{
  int i;
  node_visits[position] = 1;
  path[path_index] = position;
  path_index++;

  if(position == dest)
  {
    add_child_to_grid(path,path_index);
  }
  else
  {
    for(i = 0; i <= dest; i++)
    {
      if(adj_matrix[position][i] > 0 && node_visits[i] == 0)
      {
        get_path(i, dest, adj_matrix, node_visits, path, path_index);
      }
    }
  }

  path_index--;
  node_visits[position] = 0;
}

void calculate_paths(int **adj_matrix, int product_viability,
  int plan_duration)
{
  int i;
  int nodes_visits[plan_duration];
  int path[plan_duration];
  int path_index = 0;
  int source = 0;
  int dest = plan_duration;

  for(i = 0; i <= plan_duration; i++)
  {
    nodes_visits[i] = 0;
    path[i] = 0;
  }
  get_path(source, dest, adj_matrix, nodes_visits, path, path_index);
}

void show_results(GtkWidget *parent, int **paths,
  int product_viability, int plan_duration)
{
  GtkBuilder *builder;
  GtkWidget *label = gtk_label_new("Possible Plans");

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Machine_Maintenance/equipment.glade", NULL);

  result_window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));
  weight_text = GTK_WIDGET(gtk_builder_get_object(builder, "path_text"));
  scrolledwindow_paths = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_paths"));
  viewport_paths = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_paths"));

  gtk_widget_set_sensitive(parent, FALSE);
  gtk_window_set_transient_for(GTK_WINDOW(result_window),GTK_WINDOW(parent));

  g_signal_connect(result_window, "destroy", G_CALLBACK(on_results_destroy), NULL);

  g_object_unref(builder);

  grid_paths = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid_paths),label,0,0,1,1);

  gtk_grid_set_row_homogeneous(GTK_GRID(grid_paths),TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid_paths),TRUE);

  calculate_paths(paths, product_viability, plan_duration);

  gtk_container_add(GTK_CONTAINER(viewport_paths), grid_paths);

  gtk_widget_show_all(result_window);
}
