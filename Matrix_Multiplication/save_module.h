#include <gtk/gtk.h>
#ifndef SAVE_VIEW_H
#define SAVE_VIEW_H


void display_save_window(GtkWidget * window, int matrix_count,
int * dimensions_array);
void write_to_file();
void destroy_save_window();

#endif
