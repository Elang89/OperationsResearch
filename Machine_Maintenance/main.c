#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#include "dialogs.h"
#include "equipment_backend.h"
#include "results.h"

GtkWidget *main_window_equipment;
GtkWidget *years_field;
GtkWidget *btn_years_accept;
GtkWidget *btn_years_cancel;
GtkWidget *filechooser_window;
GtkWidget *btn_load_file;
GtkWidget *btn_cancel_file;
GtkWidget *viability_field;
GtkWidget *btn_viability_accept;
GtkWidget *btn_viability_cancel;
GtkWidget *product_cost_field;
GtkWidget *btn_product_cost_accept;
GtkWidget *btn_product_cost_cancel;
GtkWidget *viewport_costs;
GtkWidget *scrolledwindow_costs;
GtkWidget *grid_costs;
GtkWidget *viewport_results;
GtkWidget *grid_results;
GtkWidget *scrolledwindow_results;
GtkWidget *file_name_field;
GtkWidget *btn_run;
GtkWidget *btn_read_file;
GtkWidget *btn_reset;
GtkWidget *btn_maintenance_plans;
GtkWidget *btn_create_file;
GtkWidget *btn_exit;
EquipmentData *equipment_data;

void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

void display_results()
{
  GtkWidget *entry_t_number;
  GtkWidget *entry_value;
  GtkWidget *label_t_number = gtk_label_new("t");
  GtkWidget *label_function = gtk_label_new("G(t)");
  GtkWidget *label_next = gtk_label_new("Next");
  char str[200];
  char t_str[200];
  char path_str[200];
  int counter = 0;
  int array[(int)equipment_data->plan_duration+1];
  float value;
  int i;
  int j;
  int k;

  grid_results = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid_results),label_t_number,0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_results),label_function,1,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_results),label_next,2,0,1,1);

  for(i = 0; i <= equipment_data->plan_duration; i++)
  {
    entry_t_number = gtk_entry_new();
    entry_value = gtk_entry_new();
    value = equipment_data->solutions[i];

    g_object_set(entry_t_number,"editable", FALSE, NULL);
    g_object_set(entry_value, "editable", FALSE, NULL);
    sprintf(str, "%.2f", value);
    sprintf(t_str, "%d", i);
    gtk_entry_set_text(GTK_ENTRY(entry_t_number),t_str);
    gtk_entry_set_text(GTK_ENTRY(entry_value),str);
    gtk_grid_attach(GTK_GRID(grid_results),entry_t_number,0,i+1,1,1);
    gtk_grid_attach(GTK_GRID(grid_results),entry_value,1,i+1,1,1);
  }

  for(i = 0; i <= equipment_data->plan_duration; i++)
  {
    entry_value = gtk_entry_new();
    g_object_set(entry_value, "editable", FALSE, NULL);
    for(j = 0; j <= equipment_data->plan_duration; j++)
    {
      if(equipment_data->result_matrix[i][j] > 0)
      {
        array[counter] = equipment_data->result_matrix[i][j];
        counter++;
      }
    }
    strcpy(path_str,"");
    str[0] = '\0';
    for(k = 0; k < counter; k++)
    {
      sprintf(str, "%d ", array[k]);
      strcat(path_str, str);
    }
    counter = 0;

    gtk_entry_set_text(GTK_ENTRY(entry_value),path_str);
    gtk_grid_attach(GTK_GRID(grid_results),entry_value,2,i+1,1,1);
    str[0] = '\0';
    path_str[0] = '\0';
    strcpy(path_str,"");

  }

  gtk_grid_set_row_homogeneous(GTK_GRID(grid_results),TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid_results),TRUE);
  gtk_container_add(GTK_CONTAINER(viewport_results), grid_results);
  gtk_widget_show_all(scrolledwindow_results);
}

void send_data()
{
  GtkWidget* spin_button_maintenance_cost;
  GtkWidget* spin_button_resale_price;
  char * file_name;
  int status_code;
  int i;
  int plan_duration = gtk_spin_button_get_value(GTK_SPIN_BUTTON(years_field));
  int product_viability = gtk_spin_button_get_value(GTK_SPIN_BUTTON(viability_field));
  int new_product_cost = gtk_spin_button_get_value(GTK_SPIN_BUTTON(product_cost_field));

  float *maintenance_costs = malloc(plan_duration * sizeof(int)+1);
  float *resale_prices = malloc(plan_duration * sizeof(int)+1);


  for(i = 1; i <= product_viability; i++)
  {
    spin_button_maintenance_cost = gtk_grid_get_child_at(GTK_GRID(grid_costs),1,i);
    spin_button_resale_price = gtk_grid_get_child_at(GTK_GRID(grid_costs),2,i);

    maintenance_costs[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_maintenance_cost));
    resale_prices[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button_resale_price));
  }
  equipment_data = new_equipment_data(plan_duration, product_viability,
    new_product_cost, maintenance_costs, resale_prices);

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn_create_file)) == TRUE)
    {
      file_name = gtk_entry_get_text(GTK_ENTRY(file_name_field));
      status_code = write_to_file(file_name, maintenance_costs, resale_prices,
      plan_duration, product_viability, new_product_cost);

      if(status_code == 0)
      {
        error_dialog(main_window_equipment, "File name is invalid.");
      }
    }
    display_results();

}

void reset_window(GtkWidget *button, gpointer data)
{
  gtk_widget_destroy(grid_costs);
  gtk_widget_destroy(grid_results);

  gtk_entry_set_text(GTK_ENTRY(file_name_field),"");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_create_file), FALSE);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(years_field),1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(viability_field),1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(product_cost_field),1);

  gtk_widget_set_sensitive(btn_reset, FALSE);
  gtk_widget_set_sensitive(btn_maintenance_plans, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(btn_read_file, TRUE);
  gtk_widget_set_sensitive(years_field, TRUE);
  gtk_widget_set_sensitive(btn_years_accept, TRUE);
}


int verify_file_field(GtkWidget *button, gpointer data)
{
  unsigned int length = gtk_entry_get_text_length(GTK_ENTRY(file_name_field));

  if(length == 0 &&
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn_create_file)) == TRUE)
  {
    error_dialog(main_window_equipment, "File name is invalid.");
    return 0;
  }

  gtk_widget_set_sensitive(file_name_field, FALSE);
  gtk_widget_set_sensitive(btn_create_file, FALSE);
  gtk_widget_set_sensitive(grid_costs, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(btn_maintenance_plans, TRUE);
  gtk_widget_set_sensitive(btn_reset, TRUE);
  gtk_widget_set_sensitive(btn_product_cost_cancel, FALSE);
  send_data();
  return 1;
}

void create_object_grid(FileData* file_data)
{
  int i;
  int j;
  char str[20];
  int viability = gtk_spin_button_get_value(GTK_SPIN_BUTTON(viability_field));
  GtkWidget *entry;
  GtkWidget *spin_button;
  GtkWidget* label_years = gtk_label_new("Years");
  GtkWidget* label_cost = gtk_label_new("Maintenace costs in $ per year") ;
  GtkWidget* label_resale = gtk_label_new("Resale price in $ per year");

  if(file_data != NULL)
  {
    viability = file_data->product_viability;
  }

  grid_costs = gtk_grid_new();

  gtk_grid_attach(GTK_GRID(grid_costs), label_years,0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_costs), label_cost,1,0,1,1);
  gtk_grid_attach(GTK_GRID(grid_costs), label_resale,2,0,1,1);
  gtk_widget_set_halign(label_resale, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(label_cost, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(label_resale, GTK_ALIGN_CENTER);

  for(i = 1; i <= viability  ; i++)
  {
    entry = gtk_entry_new();
    sprintf(str,"%d",i);
    g_object_set(entry, "editable", FALSE, NULL);
    gtk_entry_set_text(GTK_ENTRY(entry),str);
    gtk_grid_attach(GTK_GRID(grid_costs),entry,0,i,1,1);
  }

  for(i = 1; i < 3 ; i++)
  {
    for(j = 1; j <= viability; j++)
    {
      GtkAdjustment *adjustment = gtk_adjustment_new(1,1,30000,0.01,10,0);
      GtkWidget *spin_button = gtk_spin_button_new(adjustment,0,2);
      gtk_grid_attach(GTK_GRID(grid_costs),spin_button,i,j,1,1);
    }
  }

  gtk_grid_set_row_homogeneous(GTK_GRID(grid_costs),TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid_costs),TRUE);
  gtk_container_add(GTK_CONTAINER(viewport_costs),grid_costs);
  gtk_widget_show_all(scrolledwindow_costs);

  if(file_data != NULL)
  {
    for(i = 1; i < 3; i++)
    {
      for(j = 1; j <= viability; j++)
      {
        spin_button = gtk_grid_get_child_at(GTK_GRID(grid_costs),i,j);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button),file_data->values[j-1][i-1]);
      }
    }
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(years_field), file_data->plan_duration);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(viability_field), file_data->product_viability);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(product_cost_field), file_data->new_product_cost);

    gtk_widget_set_sensitive(btn_product_cost_cancel, TRUE);
    gtk_widget_set_sensitive(product_cost_field, TRUE);
    gtk_widget_set_sensitive(btn_run, TRUE);
    gtk_widget_set_sensitive(btn_create_file, TRUE);
    gtk_widget_set_sensitive(file_name_field, TRUE);

    gtk_widget_set_sensitive(years_field, FALSE);
    gtk_widget_set_sensitive(btn_years_accept, FALSE);
    gtk_widget_set_sensitive(btn_years_cancel, FALSE);
    gtk_widget_set_sensitive(viability_field, FALSE);
    gtk_widget_set_sensitive(btn_viability_accept, FALSE);
    gtk_widget_set_sensitive(btn_viability_cancel, FALSE);
    gtk_widget_set_sensitive(product_cost_field, FALSE);
    gtk_widget_set_sensitive(btn_product_cost_accept, FALSE);
    gtk_widget_set_sensitive(btn_read_file, FALSE);

  }

}

void open_results(GtkWidget* button, gpointer data)
{
  show_results(main_window_equipment, equipment_data->result_matrix,
    equipment_data->product_viability, equipment_data->plan_duration);
}

void accept_years_value(GtkWidget *button, gpointer data)
{
  gtk_widget_set_sensitive(btn_years_cancel, TRUE);
  gtk_widget_set_sensitive(viability_field, TRUE);
  gtk_widget_set_sensitive(btn_viability_accept,TRUE);

  gtk_widget_set_sensitive(years_field, FALSE);
  gtk_widget_set_sensitive(btn_years_accept, FALSE);
  gtk_widget_set_sensitive(btn_read_file, FALSE);
}

void cancel_years_value(GtkWidget *button, gpointer data)
{
  gtk_widget_set_sensitive(btn_years_cancel, FALSE);
  gtk_widget_set_sensitive(viability_field, FALSE);
  gtk_widget_set_sensitive(btn_viability_accept,FALSE);

  gtk_widget_set_sensitive(years_field, TRUE);
  gtk_widget_set_sensitive(btn_years_accept, TRUE);
  gtk_widget_set_sensitive(btn_read_file, FALSE);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(years_field), 1);
}

void accept_viability_value(GtkWidget *button, gpointer data)
{
  float plan_duration = gtk_spin_button_get_value(GTK_SPIN_BUTTON(years_field));
  float viability = gtk_spin_button_get_value(GTK_SPIN_BUTTON(viability_field));

  if(plan_duration < viability)
  {
    error_dialog(main_window_equipment, "Viability is larger than plan duration, choose a smaller value.");
    return;
  }

  gtk_widget_set_sensitive(product_cost_field,TRUE);
  gtk_widget_set_sensitive(btn_product_cost_accept,TRUE);
  gtk_widget_set_sensitive(btn_viability_cancel,TRUE);

  gtk_widget_set_sensitive(viability_field,FALSE);
  gtk_widget_set_sensitive(btn_viability_accept, FALSE);
  gtk_widget_set_sensitive(btn_years_cancel, FALSE);
}

void cancel_viability_value(GtkWidget *button, gpointer data)
{
  gtk_widget_set_sensitive(product_cost_field,FALSE);
  gtk_widget_set_sensitive(btn_product_cost_accept,FALSE);
  gtk_widget_set_sensitive(btn_viability_cancel,FALSE);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(viability_field),1);
  gtk_widget_set_sensitive(viability_field,TRUE);
  gtk_widget_set_sensitive(btn_viability_accept, TRUE);
  gtk_widget_set_sensitive(btn_years_cancel, TRUE);
}

void accept_product_value(GtkWidget *button, gpointer data)
{
  gtk_widget_set_sensitive(btn_product_cost_cancel, TRUE);
  gtk_widget_set_sensitive(btn_run,TRUE);
  gtk_widget_set_sensitive(btn_create_file,TRUE);
  gtk_widget_set_sensitive(file_name_field, TRUE);

  gtk_widget_set_sensitive(product_cost_field, FALSE);
  gtk_widget_set_sensitive(btn_product_cost_accept, FALSE);
  gtk_widget_set_sensitive(btn_viability_cancel,FALSE);

  create_object_grid(NULL);
}

void cancel_product_value(GtkWidget* button, gpointer data)
{
  gtk_widget_set_sensitive(product_cost_field,TRUE);
  gtk_widget_set_sensitive(btn_product_cost_accept,TRUE);
  gtk_widget_set_sensitive(btn_viability_cancel,TRUE);


  gtk_widget_set_sensitive(file_name_field, FALSE);
  gtk_widget_set_sensitive(btn_product_cost_cancel, FALSE);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(product_cost_field), 1);
  gtk_entry_set_text(GTK_ENTRY(file_name_field),"");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_create_file),FALSE);

  gtk_widget_destroy(grid_costs);
}

void close_filechooser_window(GtkWidget* button, gpointer data)
{
  gtk_widget_set_sensitive(main_window_equipment, TRUE);
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
    error_dialog(main_window_equipment, "Invalid file, please choose a valid file. Files can only contain names and numbers.");
    return 0;
  }
  create_object_grid(file_data);
  file_data_destroy(file_data);
  close_filechooser_window(NULL, NULL);

  return 1;
}

void show_filechooser()
{
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Machine_Maintenance/equipment.glade", NULL);

  filechooser_window = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_window"));
  btn_load_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_load_file"));
  btn_cancel_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel_file"));

  gtk_window_set_transient_for(GTK_WINDOW(filechooser_window), GTK_WINDOW(main_window_equipment));

  g_signal_connect(btn_load_file, "clicked", G_CALLBACK(load_file_name), NULL);
  g_signal_connect(btn_cancel_file, "clicked", G_CALLBACK(close_filechooser_window), NULL);
  gtk_widget_set_sensitive(main_window_equipment, FALSE);

  g_object_unref(builder);
  gtk_widget_show(filechooser_window);
}

void assign_and_display_objects()
{
   GtkBuilder *builder;

   builder = gtk_builder_new();
   gtk_builder_add_from_file (builder, "Machine_Maintenance/equipment.glade", NULL);

   main_window_equipment = GTK_WIDGET(gtk_builder_get_object(builder, "main_window_equipment"));
   years_field = GTK_WIDGET(gtk_builder_get_object(builder, "years_field"));
   btn_years_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_years_accept"));
   btn_years_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_years_cancel"));
   viability_field = GTK_WIDGET(gtk_builder_get_object(builder, "viability_field"));
   btn_viability_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_viability_accept"));
   btn_viability_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_viability_cancel"));
   product_cost_field = GTK_WIDGET(gtk_builder_get_object(builder, "product_cost_field"));
   btn_product_cost_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_product_cost_accept"));
   btn_product_cost_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_product_cost_cancel"));
   scrolledwindow_costs = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_costs"));
   viewport_costs = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_costs"));
   viewport_results = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_results"));
   scrolledwindow_results = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_results"));
   file_name_field = GTK_WIDGET(gtk_builder_get_object(builder, "file_name_field"));
   btn_read_file = GTK_WIDGET(gtk_builder_get_object(builder,"btn_read_file"));
   btn_run = GTK_WIDGET(gtk_builder_get_object(builder, "btn_run"));
   btn_reset = GTK_WIDGET(gtk_builder_get_object(builder, "btn_reset"));
   btn_maintenance_plans = GTK_WIDGET(gtk_builder_get_object(builder, "btn_maintenance_plans"));
   btn_create_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_create_file"));
   btn_exit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_exit"));

   g_object_unref(builder);

   g_signal_connect(main_window_equipment, "destroy", G_CALLBACK(on_window_destroy),NULL);
   g_signal_connect(btn_years_accept, "clicked", G_CALLBACK(accept_years_value), NULL);
   g_signal_connect(btn_years_cancel, "clicked", G_CALLBACK(cancel_years_value), NULL);
   g_signal_connect(btn_viability_accept, "clicked", G_CALLBACK(accept_viability_value), NULL);
   g_signal_connect(btn_viability_cancel, "clicked", G_CALLBACK(cancel_viability_value), NULL);
   g_signal_connect(btn_maintenance_plans, "clicked", G_CALLBACK(open_results), NULL);
   g_signal_connect(btn_product_cost_accept, "clicked", G_CALLBACK(accept_product_value), NULL);
   g_signal_connect(btn_product_cost_cancel, "clicked", G_CALLBACK(cancel_product_value), NULL);
   g_signal_connect(btn_read_file, "clicked", G_CALLBACK(show_filechooser),NULL);
   g_signal_connect(btn_reset, "clicked", G_CALLBACK(reset_window), NULL);
   g_signal_connect(btn_run, "clicked", G_CALLBACK(verify_file_field),NULL);
   g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show(main_window_equipment);
}


int main(int argc, char* argv[])
{
  gtk_init(NULL, NULL);
  assign_and_display_objects();
  gtk_main();
}
