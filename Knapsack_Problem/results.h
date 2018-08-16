#include <gtk/gtk.h>
#ifndef RESULTS_H
#define RESULTS_H


void show_results(GtkWidget *parent, char **names, int *objects, int weight,
  int object_number, int bounty);
void on_results_destroy(GtkWidget* window, gpointer data);

#endif
