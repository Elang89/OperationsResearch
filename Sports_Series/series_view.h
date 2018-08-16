#include <gtk/gtk.h>
#ifndef SERIES_VIEW_H
#define SERIES_VIEW_H
#include "file_handler.h"


void on_window_destroy(GtkWidget *window, gpointer data);
void reset_window(GtkWidget *button, gpointer data);
void verify_file_field(GtkWidget *button, gpointer data);
int verify_team_names();
void calculate_opposite_probability(GtkWidget* spb_button, gpointer data);
void create_result_grid(double ** result_matrix);
void set_series_data();
void request_probabilities();
void close_filechooser_window(GtkWidget* button, gpointer data);
void load_view_data(FileData *file_data);
int load_file_name(GtkWidget* button, gpointer data);
void show_filechooser();
void accept(GtkWidget *button, gpointer data);
void cancel(GtkWidget *button, gpointer data);
void assign_and_display_objects();


#endif
