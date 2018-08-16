#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"
#include "graph.h"
#include "open_file.h"

void dialog_window(char * message)
{
  GtkWidget *content_area;
  GtkWidget *label;
  GtkWidget *dialog;
  dialog = gtk_dialog_new();
  label = gtk_label_new(NULL);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(gtk_widget_destroy),dialog);

  gtk_dialog_add_button(GTK_DIALOG(dialog),"OK", GTK_RESPONSE_NONE);
  gtk_label_set_markup(GTK_LABEL(label), message);
  gtk_container_add(GTK_CONTAINER(content_area),label);
  gtk_widget_show_all(dialog);

}

void buffer_char(char* temp_char)
{
	int i;
	for(i = 0; i < 100; i++)
	{
		if(number[i] == 0)
		{
			number[i] = temp_char;
			break;
		}
	}
}

void clear_buffer()
{
  int i;
  for(i = 0; i < 100; i++)
  {
    number[i] = 0;
  }
}

void close_file_window(GtkWidget *button, gpointer data)
{
  WidgetItems *OpenFileParameters = (WidgetItems*) data;


  if(gtk_widget_get_sensitive(OpenFileParameters->parent_object->btn_accept_size) == TRUE)
  {
      gtk_widget_set_sensitive(OpenFileParameters->parent_object->btn_open_file_window, TRUE);
  }
  gtk_widget_destroy(OpenFileParameters->file_window);
}

int open_search_file(GtkWidget *button, gpointer data)
{
  FILE *file;
  char *filename;
  GtkWidget *widget;
  WidgetItems *OpenFileParameters = (WidgetItems*) data;
  char * line = malloc(50);
  int matrix[100][100];
  char *input_character;
  char *temp_character;
  int row = 0;
  int column = 0;
  int column_validator;
  int iterator;
  int i;
  int j;

  GtkFileChooser *chooser = GTK_FILE_CHOOSER (OpenFileParameters->fileChooser);
  filename = gtk_file_chooser_get_filename (chooser);
  file = fopen(filename,"r");

  if(file == NULL)
  {
    close_file_window(NULL, OpenFileParameters);
    dialog_window("There was a problem opening your file.");
    return 0;
  }

  while((input_character = fgetc(file)) != EOF)
  {
    if(isdigit(input_character))
    {
      buffer_char(input_character);
      for (temp_character = fgetc(file); isdigit(temp_character);
				temp_character = fgetc(file))
			{
        buffer_char(temp_character);
			}
      iterator = atoi(number);
      matrix[column][row] = iterator;
      column++;
      clear_buffer();
      ungetc(temp_character, file);
    }
    else if (input_character == '\n')
		{
      row++;
	    column_validator = column;
      column = 0;
    }
    else if (input_character == '-')
		{
      buffer_char(input_character);
			temp_character = fgetc(file);
			if(temp_character == '-')
			{
				do
				{
					input_character = fgetc(file);
          buffer_char(input_character);
				} while (isdigit(input_character));
        iterator = atoi(number);
        matrix[column][row] = iterator;
        column++;
        clear_buffer();
			}
			else
			{
				ungetc(temp_character, file);
        continue;
			}
		}
    else if(isspace(input_character))
    {
      continue;
    }
    else
    {
      dialog_window("The file you tried to open is invalid. Files must contain numbers only.");
      close_file_window(NULL, OpenFileParameters);
      return 0;
    }
  }

  fclose(file);
  free(line);
  g_free(filename);

  if(row < 11 && column_validator < 11)
  {
    column_validator++;
    row++;
  }

  if(row > 11 || column_validator > 11)
  {
    dialog_window("This file contains an invalid matrix size. The largest size of the matrix is 10 x 10. The matrix must also be square.");
    close_file_window(NULL, OpenFileParameters);
    return 0;
  }

  for(i = 0; i < row; i++)
    for(j = 0; j < row; j++)
    {
      widget = gtk_grid_get_child_at(GTK_GRID(OpenFileParameters->parent_object->grid_matrix), i, j);
      gtk_widget_set_visible(widget, TRUE);
    }

  for(i = 1; i < row; i++)
    for(j = 1; j < row; j++)
    {
      if(i != j)
      {
        widget = gtk_grid_get_child_at(GTK_GRID(OpenFileParameters->parent_object->grid_matrix), i, j);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget),matrix[i-1][j-1]);
      }
    }
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(OpenFileParameters->parent_object->graph_size_field),row-1);

  gtk_widget_set_sensitive(OpenFileParameters->parent_object->graph_size_field, FALSE);
  gtk_widget_set_sensitive(OpenFileParameters->parent_object->btn_accept_size, FALSE);
  gtk_widget_set_sensitive(OpenFileParameters->parent_object->btn_open_file_window, FALSE);
  gtk_widget_set_sensitive(OpenFileParameters->parent_object->btn_run_floyd, TRUE);
  close_file_window(NULL, OpenFileParameters);

  return 1;
}


void show_file_window(GtkWidget* button, gpointer data) {
    GtkBuilder *builder;
    GtkWidget *open_file_window;
    WidgetItems *OpenFileParameters = malloc(sizeof(WidgetItems));
    WidgetGroup *FloydParameters = (WidgetGroup*) data;


    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Shortest_Paths/open_file.glade", NULL);

    open_file_window = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_window"));
    OpenFileParameters->file_window = open_file_window;
    OpenFileParameters->parent_object = FloydParameters;
    OpenFileParameters->btn_search_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_search_file"));
    OpenFileParameters->btn_cancel_file_window = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel_file_window"));
    OpenFileParameters->fileChooser = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_window"));
    gtk_widget_set_sensitive(button, FALSE);


    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect (OpenFileParameters->btn_search_file, "clicked", G_CALLBACK (open_search_file), OpenFileParameters);
    g_signal_connect (OpenFileParameters->btn_cancel_file_window, "clicked", G_CALLBACK(close_file_window),OpenFileParameters);

    g_object_unref(builder);

    gtk_widget_show(open_file_window);
}
