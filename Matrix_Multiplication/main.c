#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "results.h"
#include "load_module.h"
#include "types.h"
#include "save_module.h"
#include "matrix_model.h"
#include "dialogs.h"

GtkWidget *main_window_mat;
GtkWidget *btn_accept;
GtkWidget *btn_cancel;
GtkWidget *chbtn_floats;
GtkWidget *spb_matrix_number;
GtkWidget *btn_run;
GtkWidget *btn_reset;
GtkWidget *btn_dimensions;
GtkWidget *btn_create_file;
GtkWidget *btn_exit;
GtkWidget *load_window;
GtkWidget *btn_load;
GtkWidget *btn_load_cancel;
GtkWidget *file_name_field;
GtkWidget *menu_item_open;
GtkWidget *menu_item_save;
GtkWidget *menu_item_exit;
GtkWidget *grid_matrices;
GtkWidget *grid_costs;
GtkWidget *grid_results;
GtkWidget *btn_results;
GtkWidget *scrolledwindow_costs;
GtkWidget *viewport_costs;
GtkWidget *scrolledwindow_results;
GtkWidget *viewport_results;
GtkWidget *scrolledwindow_matrices;
GtkWidget *viewport_matrices;
MatrixData *matrix_data;


//This destroys window, btn_exit calls this function
void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

/*Check matrix dimensions are valid*/
int verify_matrix_data()
{
  int i;
  GtkWidget* spb_dimension_1;
  GtkWidget* spb_dimension_2;
  int value1;
  int value2;

  for(i = 1; i < matrix_data->matrix_count; i++)
  {
    spb_dimension_1 = gtk_grid_get_child_at(GTK_GRID(grid_matrices),0,i+1);
    spb_dimension_2 = gtk_grid_get_child_at(GTK_GRID(grid_matrices),1,i);

    value1 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_dimension_1));
    value2 = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_dimension_2));

    if(value1 != value2)
    {
      error_dialog(main_window_mat,"Incompatible dimensions found, matrices need to be compatible");
      return 1;
    }
  }

  return 0;
}

//This resets everything in the window, btn_reset calls this function
void reset_window(GtkWidget *button, gpointer data)
{
  gtk_widget_destroy(grid_matrices);
  gtk_widget_destroy(grid_costs);
  gtk_widget_destroy(grid_results);

  gtk_widget_set_sensitive(btn_reset, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(menu_item_save, FALSE);
  gtk_widget_set_sensitive(btn_results, FALSE);
  gtk_widget_set_sensitive(menu_item_open, TRUE);
  gtk_widget_set_sensitive(spb_matrix_number, TRUE);
  gtk_widget_set_sensitive(btn_accept, TRUE);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_matrix_number),0);
}

void display_results(int matrix_count, int cost_matrix[matrix_count][matrix_count],
  int result_matrix[matrix_count][matrix_count])
{
  grid_costs = gtk_grid_new();
  grid_results = gtk_grid_new();
  GtkWidget* entry1;
  GtkWidget* entry2;
  char str1[100];
  char str2[100];

  int i = 0;
  int j = 0;

  for(i = 0; i < matrix_count; i++)
  {
    for(j = 0; j < matrix_count; j++)
    {
      snprintf(str1, 50, "%d", cost_matrix[i][j]);
      snprintf(str2, 50, "%d", result_matrix[i][j]);
      entry1 = gtk_entry_new();
      entry2 = gtk_entry_new();
      gtk_entry_set_text(GTK_ENTRY(entry1),str1);
      gtk_entry_set_text(GTK_ENTRY(entry2),str2);
      gtk_grid_attach(GTK_GRID(grid_costs),entry1,j,i,1,1);
      gtk_grid_attach(GTK_GRID(grid_results),entry2,j,i,1,1);
      g_object_set(entry1, "editable", FALSE, NULL);
      g_object_set(entry2, "editable", FALSE, NULL);
    }
  }

  gtk_grid_set_row_spacing(GTK_GRID(grid_costs),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_costs),15);
  gtk_grid_set_row_spacing(GTK_GRID(grid_results),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_results),15);

  gtk_container_add(GTK_CONTAINER(viewport_costs),grid_costs);
  gtk_container_add(GTK_CONTAINER(viewport_results),grid_results);
  gtk_widget_show_all(scrolledwindow_costs);
  gtk_widget_show_all(scrolledwindow_results);

}

void send_data()
{
  int i = 0;
  int value = 0;
  GtkWidget* spb_dimension_1;
  GtkWidget* spb_dimension_2;

  if(verify_matrix_data())
    return;

  gtk_widget_set_sensitive(btn_cancel,FALSE);
  gtk_widget_set_sensitive(btn_reset, TRUE);
  gtk_widget_set_sensitive(menu_item_save, TRUE);
  gtk_widget_set_sensitive(btn_results, TRUE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(grid_matrices,FALSE);

  spb_dimension_1 = gtk_grid_get_child_at(GTK_GRID(grid_matrices),0,1);
  value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_dimension_1));
  matrix_data->dimensions[0] = value;

  for(i = 1; i <= matrix_data->matrix_count; i++)
  {
    spb_dimension_2 = gtk_grid_get_child_at(GTK_GRID(grid_matrices),1,i);
    value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_dimension_2));
    matrix_data->dimensions[i] = value;
  }

  calculate_results(matrix_data->matrix_count, matrix_data->cost_matrix,
    matrix_data->result_matrix, matrix_data->dimensions);
  display_results(matrix_data->matrix_count, matrix_data->cost_matrix,
    matrix_data->result_matrix);

}

//Create matrix input field
void create_matrix_grid()
{
  int number_of_matrices = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_matrix_number));
  int  i = 0;
  GtkWidget *label_matrices = gtk_label_new("Matrix Dimensions");
  GtkWidget *spb_dimension_1;
  GtkWidget *spb_dimension_2;
  GtkAdjustment *adj_number1;
  GtkAdjustment *adj_number2;

  grid_matrices = gtk_grid_new();
  matrix_data->matrix_count = number_of_matrices;


  gtk_grid_attach(GTK_GRID(grid_matrices),label_matrices,0,0,1,1);

  for(i = 1; i <= number_of_matrices; i++)
  {
    adj_number1 = gtk_adjustment_new(1,1,INT_MAX,1,10,0);
    adj_number2 = gtk_adjustment_new(1,1,INT_MAX,1,10,0);

    spb_dimension_1 = gtk_spin_button_new(adj_number1,0,0);;
    spb_dimension_2 = gtk_spin_button_new(adj_number2,0,0);

    gtk_grid_attach(GTK_GRID(grid_matrices),spb_dimension_1,0,i,1,1);
    gtk_grid_attach(GTK_GRID(grid_matrices),spb_dimension_2,1,i,1,1);
  }

  gtk_grid_set_row_spacing(GTK_GRID(grid_matrices),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_matrices),15);

  gtk_container_add(GTK_CONTAINER(viewport_matrices),grid_matrices);
  gtk_widget_show_all(scrolledwindow_matrices);
}

void call_save_window()
{
  display_save_window(main_window_mat, matrix_data->matrix_count,
  matrix_data->dimensions);
}

void call_result_window()
{
  display_result_window(main_window_mat, matrix_data->matrix_count,
    matrix_data->result_matrix, matrix_data->cost_matrix[0][matrix_data->matrix_count-1]);
}


void destroy_load_window()
{
  GtkWindow * main_window_mat = gtk_window_get_transient_for(GTK_WINDOW(load_window));
  gtk_widget_destroy(load_window);

  gtk_widget_set_sensitive(GTK_WIDGET(main_window_mat), TRUE);
}

/*Load data into window*/
void load_file_data()
{
  int number_of_matrices = 0;
  int  i = 0;
  GtkWidget *label_matrices = gtk_label_new("Matrix Dimensions");
  GtkWidget *spb_dimension_1;
  GtkWidget *spb_dimension_2;
  GtkAdjustment *adj_number_1;
  GtkAdjustment *adj_number_2;
  FileData * file_data = new_file_data();
  char * file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(load_window));
  read_from_file(file_data, file_name);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_matrix_number), file_data->matrix_count);
  number_of_matrices = file_data->matrix_count;

  matrix_data->matrix_count = number_of_matrices;

  if(file_data->status == 1)
  {
    error_dialog(main_window_mat,"Invalid File");
  }

  grid_matrices = gtk_grid_new();

  gtk_grid_attach(GTK_GRID(grid_matrices),label_matrices,0,0,1,1);

  for(i = 1; i <= number_of_matrices; i++)
  {
    adj_number_1 = gtk_adjustment_new(1,1,INT_MAX,1,10,0);
    adj_number_2 = gtk_adjustment_new(1,1,INT_MAX,1,10,0);

    spb_dimension_1 = gtk_spin_button_new(adj_number_1,0,0);
    spb_dimension_2 = gtk_spin_button_new(adj_number_2,0,0);

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_dimension_1),file_data->dimensions[i]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_dimension_2),file_data->dimensions[i+1]);
    gtk_grid_attach(GTK_GRID(grid_matrices),spb_dimension_1,0,i,1,1);
    gtk_grid_attach(GTK_GRID(grid_matrices),spb_dimension_2,1,i,1,1);
  }

  gtk_grid_set_row_spacing(GTK_GRID(grid_matrices),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_matrices),15);

  gtk_container_add(GTK_CONTAINER(viewport_matrices),grid_matrices);
  gtk_widget_show_all(scrolledwindow_matrices);

  gtk_widget_set_sensitive(btn_cancel,TRUE);
  gtk_widget_set_sensitive(btn_run,TRUE);
  gtk_widget_set_sensitive(menu_item_open, FALSE);
  gtk_widget_set_sensitive(spb_matrix_number,FALSE);
  gtk_widget_set_sensitive(btn_accept,FALSE);

  destroy_load_window();
}

void display_load_window()
{
   GtkBuilder *builder;

   gtk_widget_set_sensitive(main_window_mat, FALSE);
   builder = gtk_builder_new();
   gtk_builder_add_from_file (builder, "Matrix_Multiplication/matrices.glade", NULL);

   load_window = GTK_WIDGET(gtk_builder_get_object(builder, "load_window"));
   btn_load = GTK_WIDGET(gtk_builder_get_object(builder, "btn_load"));
   btn_load_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_load_cancel"));

   gtk_window_set_transient_for(GTK_WINDOW(load_window), GTK_WINDOW(main_window_mat));

   g_signal_connect(load_window, "destroy", G_CALLBACK(destroy_load_window), NULL);
   g_signal_connect(btn_load, "clicked", G_CALLBACK(load_file_data), NULL);
   g_signal_connect(btn_load_cancel, "clicked", G_CALLBACK(destroy_load_window), NULL);

   gtk_widget_show(load_window);
}

/*Disable buttons, call team name input verification function and finally
call result grid creation function */
void accept(GtkWidget *button, gpointer data)
{

  gtk_widget_set_sensitive(btn_cancel,TRUE);
  gtk_widget_set_sensitive(btn_run,TRUE);

  gtk_widget_set_sensitive(menu_item_open, FALSE);
  gtk_widget_set_sensitive(spb_matrix_number,FALSE);
  gtk_widget_set_sensitive(btn_accept,FALSE);

  create_matrix_grid();

}

/*Enable buttons disabled by accept, destroy grid*/
void cancel(GtkWidget* button, gpointer data)
{
  gtk_widget_destroy(grid_matrices);
  gtk_widget_set_sensitive(spb_matrix_number,TRUE);
  gtk_widget_set_sensitive(btn_accept,TRUE);
  gtk_widget_set_sensitive(menu_item_save, FALSE);

  gtk_widget_set_sensitive(menu_item_open, TRUE);
  gtk_widget_set_sensitive(btn_cancel, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);

}



int main(int argc, char * argv[])
{
  gtk_init(NULL, NULL);
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Matrix_Multiplication/matrices.glade", NULL);

  main_window_mat = GTK_WIDGET(gtk_builder_get_object(builder, "main_window_mat"));
  btn_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_accept"));
  btn_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel"));
  spb_matrix_number = GTK_WIDGET(gtk_builder_get_object(builder, "spb_matrix_number"));
  scrolledwindow_results = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_results"));
  viewport_results = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_results"));
  scrolledwindow_costs = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_costs"));
  viewport_costs = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_costs"));
  scrolledwindow_matrices = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_matrices"));
  viewport_matrices = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_matrices"));
  menu_item_save = GTK_WIDGET(gtk_builder_get_object(builder, "menu_item_save"));
  menu_item_open = GTK_WIDGET(gtk_builder_get_object(builder, "menu_item_open"));
  menu_item_exit = GTK_WIDGET(gtk_builder_get_object(builder, "menu_item_exit"));
  btn_run = GTK_WIDGET(gtk_builder_get_object(builder, "btn_run"));
  btn_reset = GTK_WIDGET(gtk_builder_get_object(builder, "btn_reset"));
  btn_results = GTK_WIDGET(gtk_builder_get_object(builder, "btn_results"));
  btn_exit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_exit"));

  g_object_unref(builder);

  g_signal_connect(main_window_mat, "destroy", G_CALLBACK(on_window_destroy),NULL);
  g_signal_connect(btn_results, "clicked", G_CALLBACK(call_result_window), NULL);
  g_signal_connect(btn_accept, "clicked", G_CALLBACK(accept), NULL);
  g_signal_connect(btn_cancel, "clicked", G_CALLBACK(cancel), NULL);
  g_signal_connect(btn_run, "clicked", G_CALLBACK(send_data), NULL);
  g_signal_connect(menu_item_exit, "activate", G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(menu_item_open, "activate", G_CALLBACK(display_load_window),NULL);
  g_signal_connect(menu_item_save, "activate", G_CALLBACK(call_save_window), NULL);
  g_signal_connect(btn_reset, "clicked", G_CALLBACK(reset_window), NULL);
  g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

  matrix_data = (MatrixData*)calloc(200, sizeof(MatrixData));

  if(!matrix_data)
  {
    printf("BST data could not be allocated, exiting.\n");
    exit(1);
  }

  gtk_widget_show(main_window_mat);
  gtk_main();
}
