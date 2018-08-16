#include "results.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget* result_window;
GtkWidget* entry_cost;
GtkWidget* entry_mult;
char final_string[200] = "";


void destroy_results()
{
  GtkWindow *window = gtk_window_get_transient_for(GTK_WINDOW(result_window));
  gtk_widget_set_sensitive(GTK_WIDGET(window), TRUE);

  gtk_widget_destroy(result_window);
}

void form_result_string(int matrix_count, int start, int end,
  int result_matrix[matrix_count][matrix_count])
{
  char base_string[200] = "";
  int k = result_matrix[start][end];


  if(start == end)
  {
    sprintf(base_string,"A%d", start+1);
    strcat(final_string,base_string);
  }
  else
  {
    strcat(final_string,"(");
    form_result_string(matrix_count,start,k,result_matrix);
    form_result_string(matrix_count,k+1,end,result_matrix);
    strcat(final_string,")");
  }

}

void display_result_window(GtkWidget* window, int matrix_count,
  int result_matrix[matrix_count][matrix_count], int final_result)
{
  GtkBuilder *builder;

  char str[200];

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Matrix_Multiplication/matrices.glade", NULL);

  result_window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));
  entry_cost = GTK_WIDGET(gtk_builder_get_object(builder, "entry_cost"));
  entry_mult = GTK_WIDGET(gtk_builder_get_object(builder, "entry_mult"));

  gtk_window_set_transient_for(GTK_WINDOW(result_window), GTK_WINDOW(window));
  gtk_widget_set_sensitive(window, FALSE);

  snprintf(str, 50, "%d", final_result);

  gtk_entry_set_text(GTK_ENTRY(entry_cost),str);

  g_signal_connect(result_window, "destroy", G_CALLBACK(destroy_results), NULL);

  form_result_string(matrix_count, 0, matrix_count-1,result_matrix);
  gtk_entry_set_text(GTK_ENTRY(entry_mult),final_string);

  gtk_widget_show(result_window);

}
