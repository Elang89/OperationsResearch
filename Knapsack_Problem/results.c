#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "results.h"


GtkWidget* result_window;
GtkWidget* scrolledwindow_objects_taken;
GtkWidget* viewport_objects_taken;
GtkWidget* result_grid;
GtkWidget* weight_text;
GtkWidget* bounty_text;

void on_results_destroy(GtkWidget* window, gpointer data)
{
  GtkWidget* parent = (GtkWidget*)gtk_window_get_transient_for(GTK_WINDOW(window));
  gtk_widget_set_sensitive(parent, TRUE);
  gtk_widget_destroy(result_window);
}
void show_results(GtkWidget *parent, char **names, int *objects, int weight, int object_number, int bounty)
{
  GtkBuilder *builder;
  GtkWidget *name;
  GtkWidget *value;
  char str[200];
  int i;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Knapsack_Problem/knapsack.glade", NULL);

  result_window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));
  weight_text = GTK_WIDGET(gtk_builder_get_object(builder, "weight_text"));
  bounty_text = GTK_WIDGET(gtk_builder_get_object(builder, "bounty_text"));
  scrolledwindow_objects_taken = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_results"));
  viewport_objects_taken = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_objects_taken"));
  result_grid = gtk_grid_new();

  gtk_builder_connect_signals(builder, NULL);


  gtk_widget_set_sensitive(parent, FALSE);
  gtk_window_set_transient_for(GTK_WINDOW(result_window),GTK_WINDOW(parent));

  g_signal_connect(result_window, "destroy", G_CALLBACK(on_results_destroy), NULL);

  g_object_unref(builder);

  sprintf(str, "%d", weight);
  gtk_entry_set_text(GTK_ENTRY(weight_text),str);

  sprintf(str, "%d", bounty);
  gtk_entry_set_text(GTK_ENTRY(bounty_text),str);


  for(i = 0; i < object_number; i++)
  {
    name = gtk_entry_new();
    value = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(name), names[i]);
    sprintf(str, "%d", objects[i+1]);
    gtk_entry_set_text(GTK_ENTRY(value), str);

    g_object_set(name, "editable", FALSE, NULL);
    g_object_set(value, "editable", FALSE, NULL);

    gtk_grid_attach(GTK_GRID(result_grid),name,0,i,1,1);
    gtk_grid_attach(GTK_GRID(result_grid),value,1,i,1,1);
  }

  gtk_grid_set_row_homogeneous(GTK_GRID(result_grid),TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(result_grid),TRUE);

  gtk_container_add(GTK_CONTAINER(viewport_objects_taken),result_grid);
  gtk_widget_show_all(result_window);
}
