#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "file_handler.h"
#include "save_module.h"
#include "bst_view.h"
#include "bst_model.h"
#include "dialogs.h"

GtkWidget *main_window_trees;
GtkWidget *load_window;
GtkWidget *btn_load;
GtkWidget *btn_load_cancel;
GtkWidget *btn_accept;
GtkWidget *btn_cancel;
GtkWidget *btn_cancel_file;
GtkWidget *chbtn_floats;
GtkWidget *node_number;
GtkWidget *btn_run;
GtkWidget *btn_reset;
GtkWidget *btn_probabilities;
GtkWidget *btn_create_file;
GtkWidget *btn_exit;
GtkWidget *file_name_field;
GtkWidget *menu_item_open;
GtkWidget *menu_item_save;
GtkWidget *menu_item_exit;
GtkWidget *grid_names;
GtkWidget *grid_nodes;
GtkWidget *grid_probabilities;
GtkWidget *grid_results;
GtkWidget *scrolledwindow_names;
GtkWidget *viewport_names;
GtkWidget *scrolledwindow_probabilities;
GtkWidget *viewport_probabilities;
GtkWidget *scrolledwindow_results;
GtkWidget *viewport_results;
GtkWidget *scrolledwindow_nodes;
GtkWidget *viewport_nodes;
BSTData *bst_data;


//This destroys window, btn_exit calls this function
void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

//This resets everything in the window, btn_reset calls this
void reset_window(GtkWidget *button, gpointer data)
{
  gtk_widget_destroy(grid_names);
  gtk_widget_destroy(grid_nodes);
  gtk_widget_destroy(grid_probabilities);
  gtk_widget_destroy(grid_results);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chbtn_floats),FALSE);

  gtk_widget_set_sensitive(btn_reset, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(menu_item_save, FALSE);
  gtk_widget_set_sensitive(menu_item_open, TRUE);
  gtk_widget_set_sensitive(node_number, TRUE);
  gtk_widget_set_sensitive(btn_accept, TRUE);
  gtk_widget_set_sensitive(chbtn_floats, TRUE);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(node_number),0);
}

int collect_total()
{
  int i;
  int value;
  GtkWidget* spin_button;

  for(i = 1; i <= bst_data->node_count; i++)
  {
    spin_button = gtk_grid_get_child_at(GTK_GRID(grid_nodes),1,i);
    value += gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));
  }
  return value;
}

/*Sort the names of each node in alphabetical order*/
void sort_names()
{
  int i = 0;
  int j = 0;
  char * temp;
  double probability;

  for(i = 0; i < bst_data->node_count; i++)
  {
    for(j = i + 1; j < bst_data->node_count; j++)
    {
      if(strcmp(bst_data->names[i],bst_data->names[j]) > 0)
      {
        temp = bst_data->names[i];
        bst_data->names[i] = bst_data->names[j];
        bst_data->names[j] = temp;
        probability = bst_data->probabilities[i];
        bst_data->probabilities[i] = bst_data->probabilities[j];
        bst_data->probabilities[j] = probability;
      }
    }
  }
}

/*Display sorted strings in alphabetical order in upper right pane*/
void display_sorted_names()
{
  int i = 0;
  char str_name[100] = "";
  char str_probability[100] = "";
  char final_str[100] = "";
  grid_names = gtk_grid_new();
  GtkWidget* entry_name;
  GtkWidget* entry_probability;
  GtkWidget* names = gtk_label_new("Names");
  GtkWidget* probabilities = gtk_label_new("Probabilities");

  gtk_grid_attach(GTK_GRID(grid_names),names,0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_names), probabilities, 1,0,1,1);

for(i = 1; i <= bst_data->node_count; i++)
  {
    sprintf(str_probability, "%.4f", bst_data->probabilities[i-1]);
    strncpy(str_name, bst_data->names[i-1],50);
    strcat(final_str, str_name);
    entry_name = gtk_entry_new();
    entry_probability = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_name),final_str);
    gtk_entry_set_text(GTK_ENTRY(entry_probability),str_probability);
    g_object_set(entry_probability,"editable",FALSE, NULL);
    g_object_set(entry_name,"editable", FALSE, NULL);
    gtk_grid_attach(GTK_GRID(grid_names),entry_name,0,i,1,1);
    gtk_grid_attach(GTK_GRID(grid_names),entry_probability,1,i,1,1);
    str_name[0] = '\0';
    final_str[0] = '\0';
  }
  gtk_grid_set_row_spacing(GTK_GRID(grid_names),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_names),15);

  gtk_container_add(GTK_CONTAINER(viewport_names),grid_names);
  gtk_widget_show_all(scrolledwindow_names);

}

void display_results(int node_count, double probability_matrix[node_count][node_count],
  int result_matrix[node_count][node_count])
{
  grid_probabilities = gtk_grid_new();
  grid_results = gtk_grid_new();
  GtkWidget* entry1;
  GtkWidget* entry2;
  char str1[100];
  char str2[100];

  int i = 0;
  int j = 0;

  for(i = 0; i < node_count; i++)
  {
    for(j = 0; j < node_count; j++)
    {
      sprintf(str1, "%.4f", probability_matrix[i][j]);
      sprintf(str2, "%d", result_matrix[i][j]);
      entry1 = gtk_entry_new();
      entry2 = gtk_entry_new();
      gtk_entry_set_text(GTK_ENTRY(entry1),str1);
      gtk_entry_set_text(GTK_ENTRY(entry2),str2);
      gtk_grid_attach(GTK_GRID(grid_probabilities),entry1,j,i,1,1);
      gtk_grid_attach(GTK_GRID(grid_results),entry2,j,i,1,1);
      g_object_set(entry1, "editable", FALSE, NULL);
      g_object_set(entry2, "editable", FALSE, NULL);
    }
  }

  gtk_grid_set_row_spacing(GTK_GRID(grid_probabilities),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_probabilities),15);
  gtk_grid_set_row_spacing(GTK_GRID(grid_results),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_results),15);

  gtk_container_add(GTK_CONTAINER(viewport_probabilities),grid_probabilities);
  gtk_container_add(GTK_CONTAINER(viewport_results),grid_results);
  gtk_widget_show_all(scrolledwindow_probabilities);
  gtk_widget_show_all(scrolledwindow_results);

}

void send_data()
{
  int i = 0;
  double value = 0;
  double total = 0;
  GtkWidget* spin_button;
  GtkWidget* entry;
  double probability_matrix[bst_data->node_count][bst_data->node_count];
  int result_matrix[bst_data->node_count][bst_data->node_count];
  bst_data->probabilities = (double*)calloc(bst_data->node_count, sizeof(double));
  bst_data->names = (char**)calloc(bst_data->node_count, sizeof(char*));

  gtk_widget_set_sensitive(btn_cancel,FALSE);
  gtk_widget_set_sensitive(btn_reset, TRUE);
  gtk_widget_set_sensitive(menu_item_save, TRUE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(grid_nodes,FALSE);

  if(!bst_data->probabilities)
  {
    printf("Probabilities could not be allocated exiting\n");
    exit(1);
  }

  if(!bst_data->names)
  {
    printf("Names at first level could not be allocated exiting\n");
    exit(1);
  }


  if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
  {
    total = collect_total();
  }

  for(i = 0; i < bst_data->node_count; i++)
  {
    bst_data->names[i] = (char*)calloc(100,sizeof(char));

    if(!bst_data->names[i])
    {
      printf("Names at second level could not be allocated exiting\n");
      exit(1);
    }
  }


  if(verify_node_data())
    return;



  for(i = 1; i <= bst_data->node_count; i++)
  {
    entry = gtk_grid_get_child_at(GTK_GRID(grid_nodes),0,i);
    spin_button = gtk_grid_get_child_at(GTK_GRID(grid_nodes),1,i);
    value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));

    if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
    {
      value = value / total;
    }
    bst_data->probabilities[i-1] = value;
    bst_data->names[i-1] = gtk_entry_get_text(GTK_ENTRY(entry));
  }
  sort_names();
  display_sorted_names();
  calculate_results(bst_data->node_count, probability_matrix, result_matrix,
    bst_data->probabilities);
  display_results(bst_data->node_count, probability_matrix, result_matrix);

}

//Create field to decide which games will have a home field advantage
void create_node_grid()
{
  int number_of_nodes = gtk_spin_button_get_value(GTK_SPIN_BUTTON(node_number));
  int  i = 0;
  GtkWidget *label_nodes = gtk_label_new("Nodes");
  GtkWidget *label_number;
  GtkWidget *entry;
  GtkWidget *spin_button;
  GtkAdjustment *adj_number;

  bst_data->node_count = number_of_nodes;

  grid_nodes = gtk_grid_new();

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
  {
    label_number = gtk_label_new("Probabilities");
  }
  else
  {
    label_number = gtk_label_new("Quantities");
  }

  gtk_grid_attach(GTK_GRID(grid_nodes),label_nodes,0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_nodes),label_number,1,0,1,1);

  for(i = 1; i <= number_of_nodes; i++)
  {
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
    {
      adj_number = gtk_adjustment_new(0,0,1,0.01,10,0);
    }
    else
    {
      adj_number = gtk_adjustment_new(1,1,100000,1,10,0);
    }

    entry = gtk_entry_new();
    spin_button = gtk_spin_button_new(adj_number,0,4);

    gtk_entry_set_max_length(GTK_ENTRY(entry),50);
    gtk_grid_attach(GTK_GRID(grid_nodes),entry,0,i,1,1);
    gtk_grid_attach(GTK_GRID(grid_nodes),spin_button,1,i,1,1);
  }

  gtk_grid_set_row_spacing(GTK_GRID(grid_nodes),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_nodes),15);

  gtk_container_add(GTK_CONTAINER(viewport_nodes),grid_nodes);
  gtk_widget_show_all(scrolledwindow_nodes);
}

/*Check if team names are empty*/
int verify_node_data()
{
  int i = 0;
  double value = 0;
  GtkWidget *entry;
  GtkWidget *spin_button;

  for(i = 1; i <= bst_data->node_count; i++)
  {
    entry = gtk_grid_get_child_at(GTK_GRID(grid_nodes),0,i);
    if(gtk_entry_get_text_length(GTK_ENTRY(entry)) == 0)
    {
      error_dialog(main_window_trees,"There is a node without a name");
      return 1;
    }
  }

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
  {
    for(i = 1; i <= bst_data->node_count; i++)
    {
      spin_button = gtk_grid_get_child_at(GTK_GRID(grid_nodes),1,i);
      value += gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button));
    }

    if(floor(value + 0.05) != 1)
    {
      error_dialog(main_window_trees, "Float values were used, but probability is greater or less than 1, insert values that sum up to 1");
      return 1;
    }
  }
  return 0;
}

void call_save_window()
{
  int option = 0;

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
  {
    option = 1;
  }

  display_save_window(main_window_trees, option, bst_data->node_count,
  bst_data->probabilities, bst_data->names);
}

/**/
void reset_on_load(FileData * file_data)
{

  if(file_data->option == 1)
  {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chbtn_floats),TRUE);
  }
  else
  {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chbtn_floats),FALSE);
  }

  gtk_widget_set_sensitive(btn_reset, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(node_number, TRUE);
  gtk_widget_set_sensitive(btn_accept, TRUE);
  gtk_widget_set_sensitive(chbtn_floats, TRUE);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(node_number),0);
}

void destroy_load_window()
{
  GtkWindow * main_window_trees = gtk_window_get_transient_for(GTK_WINDOW(load_window));
  gtk_widget_destroy(load_window);

  gtk_widget_set_sensitive(GTK_WIDGET(main_window_trees), TRUE);
}

/*Load data into window*/
void load_file_data()
{
  int number_of_nodes = 0;
  int  i = 0;
  GtkWidget *label_nodes = gtk_label_new("Nodes");
  GtkWidget *label_number;
  GtkWidget *entry;
  GtkWidget *spin_button;
  GtkAdjustment *adj_number;
  FileData * file_data = new_file_data();
  char * file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(load_window));
  read_from_file(file_data, file_name);

  reset_on_load(file_data);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(node_number), file_data->node_count);
  number_of_nodes = file_data->node_count;

  bst_data->node_count = number_of_nodes;

  if(file_data->status == 1)
  {
    error_dialog(main_window_trees,"Invalid File");
  }

  grid_nodes = gtk_grid_new();

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
  {
    label_number = gtk_label_new("Probabilities");
  }
  else
  {
    label_number = gtk_label_new("Quantities");
  }

  gtk_grid_attach(GTK_GRID(grid_nodes),label_nodes,0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_nodes),label_number,1,0,1,1);

  for(i = 1; i <= number_of_nodes; i++)
  {
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chbtn_floats)))
    {
      adj_number = gtk_adjustment_new(0,0,1,0.01,10,0);
    }
    else
    {
      adj_number = gtk_adjustment_new(1,1,100000,1,10,0);
      file_data->probabilities[i-1] *= 10000;
    }

    entry = gtk_entry_new();
    spin_button = gtk_spin_button_new(adj_number,0,2);
    gtk_entry_set_max_length(GTK_ENTRY(entry),50);

    gtk_entry_set_text(GTK_ENTRY(entry), file_data->names[i-1]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button),file_data->probabilities[i-1]);

    gtk_grid_attach(GTK_GRID(grid_nodes),entry,0,i,1,1);
    gtk_grid_attach(GTK_GRID(grid_nodes),spin_button,1,i,1,1);
  }
  gtk_grid_set_row_spacing(GTK_GRID(grid_nodes),15);
  gtk_grid_set_column_spacing(GTK_GRID(grid_nodes),15);

  gtk_container_add(GTK_CONTAINER(viewport_nodes),grid_nodes);
  gtk_widget_show_all(scrolledwindow_nodes);

  gtk_widget_set_sensitive(btn_cancel,TRUE);
  gtk_widget_set_sensitive(btn_run,TRUE);

  gtk_widget_set_sensitive(chbtn_floats,FALSE);
  gtk_widget_set_sensitive(node_number,FALSE);
  gtk_widget_set_sensitive(btn_accept,FALSE);

  destroy_load_window();
}

void display_load_window()
{
   GtkBuilder *builder;

   gtk_widget_set_sensitive(main_window_trees, FALSE);
   builder = gtk_builder_new();
   gtk_builder_add_from_file (builder, "Optimal_BSTs/trees.glade", NULL);

   load_window = GTK_WIDGET(gtk_builder_get_object(builder, "load_window"));
   btn_load = GTK_WIDGET(gtk_builder_get_object(builder, "btn_load"));
   btn_load_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_load_cancel"));

   gtk_window_set_transient_for(GTK_WINDOW(load_window), GTK_WINDOW(main_window_trees));

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
  gtk_widget_set_sensitive(chbtn_floats,FALSE);
  gtk_widget_set_sensitive(node_number,FALSE);
  gtk_widget_set_sensitive(btn_accept,FALSE);

  create_node_grid();

}

/*Enable buttons disabled by accept, destroy grid*/
void cancel(GtkWidget* button, gpointer data)
{
  gtk_widget_destroy(grid_nodes);
  gtk_widget_set_sensitive(node_number,TRUE);
  gtk_widget_set_sensitive(btn_accept,TRUE);
  gtk_widget_set_sensitive(chbtn_floats, TRUE);
  gtk_widget_set_sensitive(menu_item_save, FALSE);

  gtk_widget_set_sensitive(menu_item_open, TRUE);
  gtk_widget_set_sensitive(btn_cancel, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);

}


/*Assign objects to variables and display main window*/
void assign_and_display_objects()
{
   GtkBuilder *builder;

   builder = gtk_builder_new();
   gtk_builder_add_from_file (builder, "Optimal_BSTs/trees.glade", NULL);

   main_window_trees = GTK_WIDGET(gtk_builder_get_object(builder, "main_window_trees"));
   chbtn_floats = GTK_WIDGET(gtk_builder_get_object(builder, "chbtn_floats"));
   btn_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_accept"));
   btn_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel"));
   node_number = GTK_WIDGET(gtk_builder_get_object(builder, "node_number"));
   scrolledwindow_names = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_names"));
   viewport_names = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_names"));
   scrolledwindow_results = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_results"));
   viewport_results = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_results"));
   scrolledwindow_nodes = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_nodes"));
   viewport_nodes = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_nodes"));
   scrolledwindow_probabilities = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_probabilities"));
   viewport_probabilities = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_probabilities"));
   menu_item_save = GTK_WIDGET(gtk_builder_get_object(builder, "menu_item_save"));
   menu_item_open = GTK_WIDGET(gtk_builder_get_object(builder, "menu_item_open"));
   menu_item_exit = GTK_WIDGET(gtk_builder_get_object(builder, "menu_item_exit"));
   btn_run = GTK_WIDGET(gtk_builder_get_object(builder, "btn_run"));
   btn_reset = GTK_WIDGET(gtk_builder_get_object(builder, "btn_reset"));
   btn_exit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_exit"));

   g_object_unref(builder);

   g_signal_connect(main_window_trees, "destroy", G_CALLBACK(on_window_destroy),NULL);
   g_signal_connect(btn_accept, "clicked", G_CALLBACK(accept), NULL);
   g_signal_connect(btn_cancel, "clicked", G_CALLBACK(cancel), NULL);
   g_signal_connect(btn_run, "clicked", G_CALLBACK(send_data), NULL);
   g_signal_connect(menu_item_exit, "activate", G_CALLBACK(gtk_main_quit),NULL);
   g_signal_connect(menu_item_open, "activate", G_CALLBACK(display_load_window),NULL);
   g_signal_connect(menu_item_save, "activate", G_CALLBACK(call_save_window), NULL);
   g_signal_connect(btn_reset, "clicked", G_CALLBACK(reset_window), NULL);
   g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

   bst_data = (BSTData*)calloc(200, sizeof(BSTData));

   if(!bst_data)
   {
     printf("BST data could not be allocated, exiting.\n");
     exit(1);
   }

   gtk_widget_show(main_window_trees);
}
