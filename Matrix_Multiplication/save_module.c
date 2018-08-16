#include <gtk/gtk.h>
#include <stdlib.h>
#include "dialogs.h"
#include "save_module.h"
#include <string.h>


GtkWidget * save_window;
GtkWidget * btn_save;
GtkWidget * btn_save_cancel;
GtkWidget * file_field;
int * dimensions;
int matrix_count;
const char * file_name;


void destroy_save_window()
{
  GtkWindow * window = gtk_window_get_transient_for(GTK_WINDOW(save_window));
  gtk_widget_destroy(save_window);

  gtk_widget_set_sensitive(GTK_WIDGET(window), TRUE);
}

void write_to_file()
{
  FILE * file;
  char file_name[200];
  int i = 0;
  int text_length = gtk_entry_get_text_length(GTK_ENTRY(file_field));

  if(text_length == 0)
  {
    error_dialog(save_window, "File Name is empty");
    return;
  }

  strncpy(file_name,gtk_entry_get_text(GTK_ENTRY(file_field)),80);
  strncat(file_name,".txt", 4);
  file = fopen(file_name, "w");

  if(!file)
  {
    error_dialog(save_window, "Invalid file name");
    return;
  }

  fprintf(file, "%d\n", matrix_count);

  for(i = 0; i <= matrix_count; i++)
  {
    fprintf(file, "%d\n",dimensions[i]);
  }
  fclose(file);

  destroy_save_window();
}

void display_save_window(GtkWidget * window, int matrix_count_value,
int * dimensions_array)
{
  GtkBuilder *builder;

  matrix_count = matrix_count_value;
  dimensions = (int*)calloc(matrix_count,sizeof(int));

  dimensions = dimensions_array;

  gtk_widget_set_sensitive(window, FALSE);
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Matrix_Multiplication/matrices.glade", NULL);

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
