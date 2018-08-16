#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "results.h"
#include "dialogs.h"
#include "file_handler.h"
#include "knapsack_backend.h"


KnapsackData* knapsack_data;
GtkWidget *main_window_knapsack;
GtkWidget *filechooser_window;
GtkWidget *btn_load_file;
GtkWidget *btn_cancel_file;
GtkWidget *object_number_field;
GtkWidget *btn_object_accept;
GtkWidget *btn_object_cancel;
GtkWidget *knapsack_capacity_field;
GtkWidget *btn_capacity_accept;
GtkWidget *btn_capacity_cancel;
GtkWidget *viewport_object_details;
GtkWidget *scrolledwindow_object_details;
GtkWidget *grid_object_details;
GtkWidget *viewport_results;
GtkWidget *scrolledwindow_results;
GtkWidget *grid_results;
GtkWidget *file_name_field;
GtkWidget *btn_read_file;
GtkWidget *btn_run;
GtkWidget *btn_reset;
GtkWidget *btn_objects_taken;
GtkWidget *btn_create_file;
GtkWidget *btn_exit;

void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

void display_results(KnapsackData *knapsack_data)
{
  GdkRGBA red = {255,0,0,10};
  GdkRGBA green = {0,255,0,10};
  GtkWidget *entry;
  GtkWidget *label;
  char str[200];
  int value;
  int i;
  int j;

  grid_results = gtk_grid_new();

  for(i = 1; i <= knapsack_data->object_number; i++)
  {
    label = gtk_label_new(knapsack_data->object_names[i]);
    gtk_grid_attach(GTK_GRID(grid_results), label, i,0,1,1);
  }

  for(i = 0; i <= knapsack_data->capacity; i++)
  {
    sprintf(str, "%d", i);
    label = gtk_label_new(str);
    gtk_grid_attach(GTK_GRID(grid_results), label, 0,i+1,1,1);
  }

  for(i = 1; i <= knapsack_data->object_number; i++)
  {
    for(j = 0; j <= knapsack_data->capacity; j++)
    {
      entry = gtk_entry_new();
      g_object_set(entry, "editable", FALSE, NULL);
      value = knapsack_data->data_matrix[i][j];
      sprintf(str, "%d", value);
      gtk_entry_set_text(GTK_ENTRY(entry),str);
      if(knapsack_data->color_matrix[i][j] == 0)
      {
        gtk_widget_override_color(entry, GTK_STATE_NORMAL, &red);
      }
      else
      {
        gtk_widget_override_color(entry, GTK_STATE_NORMAL, &green);
      }
      gtk_grid_attach(GTK_GRID(grid_results),entry,i,j+1,1,1);
    }
  }

  gtk_grid_set_row_homogeneous(GTK_GRID(grid_results),TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid_results),TRUE);
  gtk_container_add(GTK_CONTAINER(viewport_results), grid_results);
  gtk_widget_show_all(scrolledwindow_results);
}

void send_data()
{
  GtkWidget* entry;
  GtkWidget* spin_button_cost;
  GtkWidget* spin_button_weight;
  GtkWidget* spin_button_amount;
  int i;
  int status_code;
  int object_number = gtk_spin_button_get_value(GTK_SPIN_BUTTON(object_number_field));
  int capacity = gtk_spin_button_get_value(GTK_SPIN_BUTTON(knapsack_capacity_field));
  const char *str;
  char *file_name;
  char **names = malloc(object_number * sizeof(char*)+1);
  int *values = malloc(object_number * sizeof(int)+1);
  int *weights = malloc(object_number * sizeof(int)+1);
  int *amounts = malloc(object_number * sizeof(int)+1);

  for(i = 0; i <= object_number; i++)
  {
    names[i] = malloc(20 * sizeof(char));
  }

  for(i = 1; i <= object_number; i++)
  {

    entry = gtk_grid_get_child_at(GTK_GRID(grid_object_details),0,i);
    spin_button_cost = gtk_grid_get_child_at(GTK_GRID(grid_object_details),1,i);
    spin_button_weight = gtk_grid_get_child_at(GTK_GRID(grid_object_details),2,i);
    spin_button_amount = gtk_grid_get_child_at(GTK_GRID(grid_object_details),3,i);
    str = gtk_entry_get_text(GTK_ENTRY(entry));

    strcpy(names[i],str);
    values[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_cost));
    weights[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_weight));
    amounts[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_amount));
  }
  knapsack_data = knapsack_data_new(names, values, weights, amounts, capacity, object_number);

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn_create_file)) == TRUE)
  {
    file_name = gtk_entry_get_text(GTK_ENTRY(file_name_field));
    status_code = write_to_file(file_name, names, values, weights, amounts, object_number, capacity);

    if(status_code == 0)
    {
      error_dialog(main_window_knapsack, "File name is invalid.");
    }
  }

  display_results(knapsack_data);
}

void reset_window(GtkWidget *button, gpointer data)
{
  gtk_widget_destroy(grid_object_details);
  gtk_widget_destroy(grid_results);

  gtk_entry_set_text(GTK_ENTRY(file_name_field),"");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_create_file), FALSE);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(knapsack_capacity_field),1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(object_number_field),1);

  gtk_widget_set_sensitive(btn_reset, FALSE);
  gtk_widget_set_sensitive(btn_objects_taken, FALSE);
  gtk_widget_set_sensitive(btn_read_file, TRUE);
  gtk_widget_set_sensitive(object_number_field, TRUE);
  gtk_widget_set_sensitive(btn_object_accept, TRUE);
}

int verify_file_field(GtkWidget *button, gpointer data)
{
  GtkWidget *entry;
  int object_number = gtk_spin_button_get_value(GTK_SPIN_BUTTON(object_number_field));
  int i;
  unsigned int length = gtk_entry_get_text_length(GTK_ENTRY(file_name_field));


  if(length == 0 &&
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn_create_file)) == TRUE)
  {
    error_dialog(main_window_knapsack,"File was requested but file name field is empty.");
    return 0;
  }

  for(i = 1; i <= object_number; i++)
  {
    entry = gtk_grid_get_child_at(GTK_GRID(grid_object_details), 0, i);
    length = gtk_entry_get_text_length(GTK_ENTRY(entry));

    if(length == 0)
    {
      error_dialog(main_window_knapsack, "An object has no name, please add a name.");
      return 0;
    }
  }

  gtk_widget_set_sensitive(file_name_field, FALSE);
  gtk_widget_set_sensitive(btn_create_file, FALSE);
  gtk_widget_set_sensitive(grid_object_details, FALSE);
  gtk_widget_set_sensitive(btn_capacity_cancel, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(btn_reset, TRUE);
  gtk_widget_set_sensitive(btn_objects_taken, TRUE);
  send_data();
  return 1;
}

void create_object_grid(FileData * file_data)
{
  int i;
  int j;
  int object_number = gtk_spin_button_get_value(GTK_SPIN_BUTTON(object_number_field));
  GtkWidget* entry;
  GtkWidget* spin_button;
  GtkWidget* label_name = gtk_label_new("Name");
  GtkWidget* label_value = gtk_label_new("Value in $") ;
  GtkWidget* label_weight = gtk_label_new("Weight in Kg");
  GtkWidget* label_amount = gtk_label_new("Amount of Objects");

  grid_object_details = gtk_grid_new();

  gtk_grid_attach(GTK_GRID(grid_object_details), label_name,0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_object_details), label_value,1,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_object_details), label_weight,2,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_object_details), label_amount,3,0,1,1);
  gtk_widget_set_halign(label_name, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(label_value, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(label_weight, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(label_amount, GTK_ALIGN_CENTER);

  if(file_data != NULL)
  {
    object_number = file_data->object_number;
  }

  for(i = 1; i <= object_number ; i++)
  {
    GtkWidget *entry = gtk_entry_new();
    g_object_set(entry, "editable", TRUE, NULL);
    gtk_entry_set_max_length(GTK_ENTRY(entry),20);
    gtk_grid_attach(GTK_GRID(grid_object_details),entry,0,i,1,1);
  }

  for(i = 1; i < 4 ; i++)
  {
    for(j = 1; j <= object_number; j++)
    {
      GtkAdjustment *adjustment = gtk_adjustment_new(1,1,30000,1,10,0);
      GtkWidget *spin_button = gtk_spin_button_new(adjustment,0,0);
      gtk_grid_attach(GTK_GRID(grid_object_details),spin_button,i,j,1,1);
    }
  }

  gtk_grid_set_row_homogeneous(GTK_GRID(grid_object_details),TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid_object_details),TRUE);
  gtk_container_add(GTK_CONTAINER(viewport_object_details),grid_object_details);
  gtk_widget_show_all(scrolledwindow_object_details);

  if(file_data != NULL)
  {
    for(i = 1; i <= file_data->object_number; i++)
    {
      entry = gtk_grid_get_child_at(GTK_GRID(grid_object_details),0,i);
      gtk_entry_set_text(GTK_ENTRY(entry), file_data->names[i-1]);
    }

    for(i = 1; i < 4; i++)
    {
      for(j = 1; j <= file_data->object_number; j++)
      {
        spin_button = gtk_grid_get_child_at(GTK_GRID(grid_object_details),i,j);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button),file_data->values[j-1][i-1]);
      }
    }
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(object_number_field),file_data->object_number);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(knapsack_capacity_field),file_data->capacity);

    gtk_widget_set_sensitive(btn_capacity_cancel, TRUE);
    gtk_widget_set_sensitive(knapsack_capacity_field, TRUE);
    gtk_widget_set_sensitive(btn_capacity_accept,TRUE);
    gtk_widget_set_sensitive(btn_run,TRUE);
    gtk_widget_set_sensitive(btn_create_file,TRUE);
    gtk_widget_set_sensitive(file_name_field, TRUE);

    gtk_widget_set_sensitive(object_number_field, FALSE);
    gtk_widget_set_sensitive(btn_object_accept, FALSE);
    gtk_widget_set_sensitive(btn_read_file, FALSE);
    gtk_widget_set_sensitive(btn_object_cancel, FALSE);
    gtk_widget_set_sensitive(knapsack_capacity_field, FALSE);
    gtk_widget_set_sensitive(btn_capacity_accept, FALSE);
  }

}

void accept_object_value(GtkWidget *button, gpointer data)
{
  gtk_widget_set_sensitive(btn_object_cancel, TRUE);
  gtk_widget_set_sensitive(knapsack_capacity_field, TRUE);
  gtk_widget_set_sensitive(btn_capacity_accept,TRUE);

  gtk_widget_set_sensitive(object_number_field, FALSE);
  gtk_widget_set_sensitive(btn_object_accept, FALSE);
  gtk_widget_set_sensitive(btn_read_file, FALSE);
}

void cancel_object_value(GtkWidget *button, gpointer data)
{
  gtk_widget_set_sensitive(btn_object_cancel, FALSE);
  gtk_widget_set_sensitive(knapsack_capacity_field, FALSE);
  gtk_widget_set_sensitive(btn_capacity_accept,FALSE);

  gtk_widget_set_sensitive(object_number_field, TRUE);
  gtk_widget_set_sensitive(btn_object_accept, TRUE);
  gtk_widget_set_sensitive(btn_read_file, TRUE);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(object_number_field), 1);
}

void accept_capacity_value(GtkWidget *button, gpointer data)
{
  gtk_widget_set_sensitive(btn_capacity_cancel, TRUE);
  gtk_widget_set_sensitive(knapsack_capacity_field, TRUE);
  gtk_widget_set_sensitive(btn_capacity_accept,TRUE);
  gtk_widget_set_sensitive(btn_run,TRUE);
  gtk_widget_set_sensitive(btn_create_file,TRUE);
  gtk_widget_set_sensitive(file_name_field, TRUE);

  gtk_widget_set_sensitive(btn_object_cancel, FALSE);
  gtk_widget_set_sensitive(knapsack_capacity_field, FALSE);
  gtk_widget_set_sensitive(btn_capacity_accept, FALSE);

  create_object_grid(NULL);
}

void open_results(GtkWidget* button, gpointer data)
{
  show_results(main_window_knapsack, knapsack_data->objects_taken_names,
    knapsack_data->objects_taken, knapsack_data->knapsack_final_weight,
    knapsack_data->taken,
    knapsack_data->data_matrix[knapsack_data->object_number][knapsack_data->capacity]);
}

void cancel_capacity_value(GtkWidget* button, gpointer data)
{
  gtk_widget_set_sensitive(btn_capacity_cancel, FALSE);
  gtk_widget_set_sensitive(knapsack_capacity_field, FALSE);
  gtk_widget_set_sensitive(btn_capacity_accept,FALSE);
  gtk_widget_set_sensitive(btn_run,FALSE);
  gtk_widget_set_sensitive(btn_create_file,FALSE);
  gtk_widget_set_sensitive(file_name_field, FALSE);

  gtk_widget_set_sensitive(btn_object_cancel, TRUE);
  gtk_widget_set_sensitive(knapsack_capacity_field, TRUE);
  gtk_widget_set_sensitive(btn_capacity_accept, TRUE);

  gtk_entry_set_text(GTK_ENTRY(file_name_field),"");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_create_file),FALSE);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(knapsack_capacity_field), 1);

  gtk_widget_destroy(grid_object_details);
}

void close_filechooser_window(GtkWidget* button, gpointer data)
{
  gtk_widget_set_sensitive(main_window_knapsack, TRUE);
  gtk_widget_destroy(filechooser_window);
}


int load_file_name(GtkWidget* button, gpointer data)
{
  char * file_name;
  FileData *file_data;
  file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser_window));
  file_data = read_from_file(file_name);

  if(file_data->status == 0)
  {
    error_dialog(main_window_knapsack, "Invalid file, please choose a valid file. Files can only contain names and numbers.");
    file_data_destroy(file_data);
    return 0;
  }
  create_object_grid(file_data);
  close_filechooser_window(NULL, NULL);

  return 1;
}

void show_filechooser()
{
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Knapsack_Problem/knapsack.glade", NULL);

  filechooser_window = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_window"));
  btn_load_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_load_file"));
  btn_cancel_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel_file"));

  gtk_window_set_transient_for(GTK_WINDOW(filechooser_window), GTK_WINDOW(main_window_knapsack));

  g_signal_connect(btn_load_file, "clicked", G_CALLBACK(load_file_name), NULL);
  g_signal_connect(btn_cancel_file, "clicked", G_CALLBACK(close_filechooser_window), NULL);
  gtk_widget_set_sensitive(main_window_knapsack, FALSE);

  g_object_unref(builder);
  gtk_widget_show(filechooser_window);
}




int main(int argc, char* argv[])
{
  gtk_init(NULL, NULL);
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Knapsack_Problem/knapsack.glade", NULL);

  main_window_knapsack = GTK_WIDGET(gtk_builder_get_object(builder, "main_window_knapsack"));
  object_number_field = GTK_WIDGET(gtk_builder_get_object(builder, "object_number_field"));
  btn_object_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_object_accept"));
  btn_object_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_object_cancel"));
  knapsack_capacity_field = GTK_WIDGET(gtk_builder_get_object(builder, "knapsack_capacity_field"));
  btn_capacity_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_capacity_accept"));
  btn_capacity_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_capacity_cancel"));
  scrolledwindow_object_details = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_object_details"));
  viewport_object_details = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_object_details"));
  scrolledwindow_results = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_results"));
  viewport_results = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_results"));
  file_name_field = GTK_WIDGET(gtk_builder_get_object(builder, "file_name_field"));
  btn_run = GTK_WIDGET(gtk_builder_get_object(builder, "btn_run"));
  btn_read_file = GTK_WIDGET(gtk_builder_get_object(builder,"btn_read_file"));
  btn_reset = GTK_WIDGET(gtk_builder_get_object(builder, "btn_reset"));
  btn_objects_taken = GTK_WIDGET(gtk_builder_get_object(builder, "btn_objects_taken"));
  btn_create_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_create_file"));
  btn_exit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_exit"));

  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  g_signal_connect(main_window_knapsack, "destroy", G_CALLBACK(on_window_destroy),NULL);
  g_signal_connect(btn_object_accept, "clicked", G_CALLBACK(accept_object_value), NULL);
  g_signal_connect(btn_object_cancel, "clicked", G_CALLBACK(cancel_object_value), NULL);
  g_signal_connect(btn_capacity_accept, "clicked", G_CALLBACK(accept_capacity_value), NULL);
  g_signal_connect(btn_capacity_cancel, "clicked", G_CALLBACK(cancel_capacity_value), NULL);
  g_signal_connect(btn_run, "clicked", G_CALLBACK(verify_file_field), NULL);
  g_signal_connect(btn_objects_taken, "clicked", G_CALLBACK(open_results), NULL);
  g_signal_connect(btn_read_file, "clicked", G_CALLBACK(show_filechooser),NULL);
  g_signal_connect(btn_reset, "clicked", G_CALLBACK(reset_window), NULL);
  g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show(main_window_knapsack);
  gtk_main();

  return 0;
}
