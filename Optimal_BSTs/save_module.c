#include <gtk/gtk.h>
#include <stdlib.h>
#include "dialogs.h"
#include "save_module.h"
#include <string.h>


GtkWidget * save_window;
GtkWidget * btn_save;
GtkWidget * btn_save_cancel;
GtkWidget * file_field;


void destroy_save_window()
{
  GtkWindow * window = gtk_window_get_transient_for(GTK_WINDOW(save_window));
  gtk_widget_destroy(save_window);

  gtk_widget_set_sensitive(GTK_WIDGET(window), TRUE);
}

void write_to_file()
{
  FILE * file;
  int i = 0;
  int text_length = gtk_entry_get_text_length(GTK_ENTRY(file_field));

  if(text_length == 0)
  {
    error_dialog(save_window, "File Name is empty");
    return;
  }

  file_name = gtk_entry_get_text(GTK_ENTRY(file_field));
  strcat(file_name, ".txt");
  file = fopen(file_name, "w");

  if(!file)
  {
    error_dialog(save_window, "Invalid file name");
    return;
  }

  fprintf(file, "%d\n", node_count);
  fprintf(file, "%d\n", option);

  for(i = 0; i < node_count; i++)
  {
    fprintf(file, "%s %.4f\n", names[i], probabilities[i]);
  }
  fclose(file);
  destroy_save_window();
}

void display_save_window(GtkWidget * window, int option_value, int node_count_value,
double * probability_array, char ** names_array)
{
  GtkBuilder *builder;
  int i = 0;

  option = option_value;
  node_count = node_count_value;

  names = (char**)calloc(node_count,sizeof(char*));
  probabilities = (double*)calloc(node_count,sizeof(double));

  for(i = 0; i < node_count; i++)
  {
    names[i] = (char*)calloc(node_count, sizeof(char));
  }

  probabilities = probability_array;
  names = names_array;

  gtk_widget_set_sensitive(window, FALSE);
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Optimal_BSTs/trees.glade", NULL);

  save_window = GTK_WIDGET(gtk_builder_get_object(builder, "save_window"));
  btn_save = GTK_WIDGET(gtk_builder_get_object(builder, "btn_save"));
  btn_save_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_save_cancel"));
  file_field = GTK_WIDGET(gtk_builder_get_object(builder,"file_field"));

  gtk_window_set_transient_for(GTK_WINDOW(save_window), GTK_WINDOW(window));

  g_signal_connect(save_window, "destroy", G_CALLBACK(destroy_save_window), NULL);
  g_signal_connect(btn_save, "clicked", G_CALLBACK(write_to_file), NULL);
  g_signal_connect(btn_save_cancel, "clicked", G_CALLBACK(destroy_save_window), NULL);

  gtk_widget_show(save_window);
}
