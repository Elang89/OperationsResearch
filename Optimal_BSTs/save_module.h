#include <gtk/gtk.h>
#ifndef SAVE_VIEW_H
#define SAVE_VIEW_H

double * probabilities;
int option;
int node_count;
char * file_name;
char ** names;

void display_save_window(GtkWidget * window, int option, int node_count,
double * probabilities, char ** names);
void write_to_file();
void destroy_save_window();

#endif
