#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "structs.h"
#include "open_file.h"


void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

void on_node_entry_edited(GtkEntry *entry, gpointer data)
{
  GtkEntry* opposite_text_field = (GtkEntry*) data;
  const char* text = gtk_entry_get_text(entry);

  gtk_entry_set_text(opposite_text_field,text);

}

void on_results_close(GtkWidget *window, gpointer data)
{
  TableWindowWidgets *result_widget_group  = (TableWindowWidgets*) data;

  gtk_widget_set_sensitive(result_widget_group->parent_object->btn_run_floyd, TRUE);
  gtk_widget_set_sensitive(result_widget_group->parent_object->btn_cancel_size, TRUE);
  gtk_widget_set_sensitive(result_widget_group->parent_object->btn_run_floyd, TRUE);
  gtk_widget_set_sensitive(result_widget_group->parent_object->grid_matrix, TRUE);

}
int find_shortest_route(GtkWidget* button, gpointer data)
{
  RouteWidgets *widgets = (RouteWidgets*) data;
  char str[200];
  char* source_text;
  char* dest_text;
  int index;
  int source_id;
  int dest_id;

  source_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->source_combo));
  dest_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->dest_combo));;

  source_id = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->source_combo));
  dest_id = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->dest_combo));

  if(dest_id == source_id)
  {
    gtk_entry_set_text(GTK_ENTRY(widgets->route_text), "The same node was selected, the distance is 0.");
    return 0;
  }
  else
  {
    index = widgets->parent_object->matrix[source_id][dest_id];
    if(index > 10)
    {
      index--;
    }
    strcpy(str,widgets->parent_object->names[source_id+1]);
    strcat(str,"->");
    while(index != 0)
    {
      index--;
      strcat(str,widgets->parent_object->names[index+1]);
      index = widgets->parent_object->matrix[index][dest_id];
      strcat(str,"->");
    }
    strcat(str,widgets->parent_object->names[dest_id+1]);
    gtk_entry_set_text(GTK_ENTRY(widgets->route_text),str);
  }
  return 0;
}


void show_routes_window(GtkWidget* widget, gpointer user_data)
{
  GtkBuilder *builder;
  GtkWidget *routes_window;
  TableWindowWidgets *result_widget_group = (TableWindowWidgets*) user_data;
  RouteWidgets *widgets = malloc(sizeof(RouteWidgets));
  char str[1000];
  int temp_value;
  int i;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Shortest_Paths/floyd_results_table.glade", NULL);

  routes_window = GTK_WIDGET(gtk_builder_get_object(builder, "routes_window"));
  widgets->parent_object = result_widget_group;
  widgets->btn_calc_route = GTK_WIDGET(gtk_builder_get_object(builder, "btn_calc_route"));
  widgets->source_combo = GTK_WIDGET(gtk_builder_get_object(builder, "source_combo"));
  widgets->dest_combo = GTK_WIDGET(gtk_builder_get_object(builder, "dest_combo"));
  widgets->route_text = GTK_WIDGET(gtk_builder_get_object(builder, "route_text"));


  gtk_builder_connect_signals(builder, NULL);
  g_signal_connect (widgets->btn_calc_route, "clicked", G_CALLBACK (find_shortest_route),widgets);

  g_object_unref(builder);

  for(i = 1; i <= widgets->parent_object->graph->node_count; i++)
  {
    temp_value = i - 1;
    sprintf(str, "%d", temp_value);
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(widgets->source_combo),i-1,str, widgets->parent_object->names[i]);
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(widgets->dest_combo),i-1,str, widgets->parent_object->names[i]);
  }
  gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->source_combo),0);
  gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->dest_combo),0);

  gtk_widget_show(routes_window);
}

void display_tables(TableWindowWidgets *result_widget_group, Graph* graph)
{
  GtkWidget *grid;
  GtkWidget *row_entry;
  GtkWidget *column_entry;
  GtkWidget *entry;
  int value = graph->node_count + 1;
  int i;
  int j;
  int k;

  result_widget_group->current_table_number = 0;

  for(i = 0; i < value; i++)
  {
    grid = result_widget_group->grid_tables[i];
    gtk_widget_set_visible(grid, TRUE);
    for(j = 1; j < value; j++)
    {
      row_entry = gtk_grid_get_child_at(GTK_GRID(grid),j,0);
      column_entry = gtk_grid_get_child_at(GTK_GRID(grid),0,j);
      gtk_entry_set_text(GTK_ENTRY(row_entry),graph->names[j]);
      gtk_entry_set_text(GTK_ENTRY(column_entry),graph->names[j]);
      gtk_widget_set_visible(row_entry, TRUE);
      gtk_widget_set_visible(column_entry, TRUE);
    }
  }

  grid = result_widget_group->grid_tables[0];

  for(i = 1; i < value; i++)
  {
    for(j = 1; j < value; j++)
    {
      entry = gtk_grid_get_child_at(GTK_GRID(grid),i,j);
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry), 0);
    }
  }

  for(i = 0; i < value; i++)
  {
    grid = result_widget_group->grid_tables[i];
    gtk_widget_set_visible(grid, TRUE);
    for(j = 1; j < value; j++)
    {
      for(k = 1; k < value; k++)
      {
        entry = gtk_grid_get_child_at(GTK_GRID(grid),j,k);
        gtk_widget_set_visible(entry, TRUE);
      }
    }
  }
}

void floyd_warshall(GtkWidget *button, gpointer data)
{
  TableWindowWidgets *result_widget_group = (TableWindowWidgets*) data;
  int i;
  int j;
  char str[1000];
  int temp_value;
  Graph *graph = result_widget_group->graph;
  GtkWidget* grid;
  GtkWidget* entry;
  GtkWidget* p_entry;
  GdkRGBA red = {255,0,0,10};
  int grid_number = result_widget_group->current_table_number+1;
  int table_number = result_widget_group->current_table_number;

  grid = result_widget_group->grid_tables[grid_number];

  if(result_widget_group->matrix_init != 1)
  {
    for(i = 0; i < graph->node_count; i++)
    {
      for(j = 0; j < graph->node_count; j++)
      {
        result_widget_group->matrix[i][j] = 0;
      }
    }
    result_widget_group->matrix_init = 1;
  }

  for(i = 0; i < graph->node_count; i++)
  {
    for(j = 0; j < graph->node_count; j++)
    {
      entry = gtk_grid_get_child_at(GTK_GRID(grid),i+1, j+1);
      if(graph->adj_matrix[j][i] >
        graph->adj_matrix[j][table_number] + graph->adj_matrix[table_number][i])
      {
        p_entry = gtk_grid_get_child_at(GTK_GRID(result_widget_group->p_grid), i+1, j+1);
        graph->adj_matrix[j][i] = graph->adj_matrix[j][table_number] + graph->adj_matrix[table_number][i];
        temp_value = graph->adj_matrix[j][i];
        sprintf(str, "%d", temp_value);
        gtk_entry_set_text(GTK_ENTRY(entry), str);
        gtk_widget_override_color(entry, GTK_STATE_NORMAL, &red);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(p_entry), table_number+1);
        result_widget_group->matrix[j][i] = table_number+1;
      }
      else
      {
        temp_value = graph->adj_matrix[j][i];
        sprintf(str, "%d", temp_value);
        gtk_entry_set_text(GTK_ENTRY(entry), str);
      }
    }
  }
  result_widget_group->current_table_number++;

  if(result_widget_group->current_table_number == graph->node_count)
  {
    for(i = 1; i <= graph->node_count; i++)
    {
      strcpy(result_widget_group->names[i], graph->names[i]);
    }

    gtk_widget_set_sensitive(result_widget_group->btn_next_table,FALSE);
    gtk_widget_set_sensitive(result_widget_group->btn_shortest_paths,TRUE);
    g_signal_connect(result_widget_group->btn_shortest_paths, "clicked", G_CALLBACK(show_routes_window), result_widget_group);
  }
}

void show_table_window(Graph* graph, WidgetGroup *widget_group)
{
  TableWindowWidgets* result_widget_group = malloc(sizeof(TableWindowWidgets));
  GtkBuilder *builder;
  GtkWidget *table_window;
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Shortest_Paths/floyd_results_table.glade", NULL);

  table_window = GTK_WIDGET(gtk_builder_get_object(builder, "results_windows"));
  result_widget_group->grid_tables[0] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_TableP"));
  result_widget_group->grid_tables[1] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table1"));
  result_widget_group->grid_tables[2] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table2"));
  result_widget_group->grid_tables[3] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table3"));
  result_widget_group->grid_tables[4] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table4"));
  result_widget_group->grid_tables[5] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table5"));
  result_widget_group->grid_tables[6] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table6"));
  result_widget_group->grid_tables[7] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table7"));
  result_widget_group->grid_tables[8] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table8"));
  result_widget_group->grid_tables[9] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table9"));
  result_widget_group->grid_tables[10] = GTK_WIDGET(gtk_builder_get_object(builder, "grid_Table10"));
  result_widget_group->btn_next_table = GTK_WIDGET(gtk_builder_get_object(builder, "btn_next_table"));
  result_widget_group->btn_shortest_paths = GTK_WIDGET(gtk_builder_get_object(builder, "btn_shortest_paths"));
  result_widget_group->p_grid = result_widget_group->grid_tables[0];
  result_widget_group->graph = graph;
  result_widget_group->parent_object = widget_group;

  g_signal_connect(result_widget_group->btn_next_table, "clicked", G_CALLBACK(floyd_warshall), result_widget_group);
  g_signal_connect (table_window, "destroy",G_CALLBACK (on_results_close), result_widget_group);
  gtk_widget_show(table_window);
  display_tables(result_widget_group,graph);
}


void create_matrix(GtkWidget *button, gpointer data)
{
  WidgetGroup *FloydParameters = (WidgetGroup*) data;
  GtkWidget *widget;
  int i;
  int j;
  int value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(FloydParameters->graph_size_field));
  value++;

  for(i = 0; i < value; i++)
    for(j = 0; j < value; j++)
    {
      widget = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix), i, j);
      gtk_widget_set_visible(widget, TRUE);
    }

  gtk_widget_set_sensitive(FloydParameters->graph_size_field, FALSE);
  gtk_widget_set_sensitive(FloydParameters->btn_accept_size, FALSE);
  gtk_widget_set_sensitive(FloydParameters->btn_open_file_window, FALSE);
  gtk_widget_set_sensitive(FloydParameters->btn_run_floyd, TRUE);

}

void cancel_matrix(GtkWidget *button, gpointer data)
{
  WidgetGroup *FloydParameters = (WidgetGroup*) data;
  GtkWidget *widget;
  GtkWidget *row_entry;
  GtkWidget *column_entry;
  const char names[11][11] = {"","A","B","C","D","E","F","G","H","I","J"};
  int value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(FloydParameters->graph_size_field));
  int i;
  int j;
  value++;

  for(i = 1; i < value ; i++)
    for(j = 1; j < value; j++)
    {
      widget = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix), i, j);
      gtk_widget_set_visible(widget, FALSE);
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget),0);
    }

  for(i = 1; i < value; i++)
  {
    row_entry = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix), 0, i);
    column_entry = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix), i, 0);
    gtk_widget_set_visible(row_entry, FALSE);
    gtk_widget_set_visible(column_entry, FALSE);
    gtk_entry_set_text(GTK_ENTRY(row_entry),names[i]);
    gtk_entry_set_text(GTK_ENTRY(column_entry),names[i]);
  }

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(FloydParameters->graph_size_field), 1);

  gtk_widget_set_sensitive(FloydParameters->graph_size_field, TRUE);
  gtk_widget_set_sensitive(FloydParameters->btn_accept_size, TRUE);
  gtk_widget_set_sensitive(FloydParameters->btn_open_file_window, TRUE);
  gtk_widget_set_sensitive(FloydParameters->btn_run_floyd, FALSE);


}

int plot_graph(GtkWidget *widget, gpointer data)
{
  WidgetGroup *FloydParameters = (WidgetGroup*) data;
  GtkWidget *spin_widget;
  GtkWidget *label;
  GtkWidget *dialog;
  GtkWidget *content_area;
  GtkWidget *row_entry;
  const char* text1;
  const char* text2;
  int graph_size = gtk_spin_button_get_value(GTK_SPIN_BUTTON(FloydParameters->graph_size_field));
  int value;
  Graph *graph = new_graph(graph_size);
  int i;
  int j;
  int cellValue;

  for(i = 1; i < graph_size; i++)
  {
    row_entry = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix),i,0);
    text1 = gtk_entry_get_text(GTK_ENTRY(row_entry));

    for(j = i + 1; j < graph_size; j++)
    {
      row_entry = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix),j,0);
      text2 = gtk_entry_get_text(GTK_ENTRY(row_entry));

      if(strcmp(text1,text2) == 0)
      {
        dialog = gtk_dialog_new();
        label = gtk_label_new(NULL);
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(gtk_widget_destroy),dialog);

        gtk_dialog_add_button(GTK_DIALOG(dialog),"OK", GTK_RESPONSE_NONE);
        gtk_label_set_markup(GTK_LABEL(label), "<big>There are nodes with the same names, please edit them.</big>");
        gtk_container_add(GTK_CONTAINER(content_area),label);
        gtk_widget_show_all(dialog);
        return 1;
      }
    }
  }


  value = graph_size + 1;

  for(i = 1; i < value; i++)
  {
    for(j = 1; j < value; j++)
    {
      spin_widget = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix),i,j);
      cellValue = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_widget));
      insert_edge(graph,i-1,j-1,cellValue);
    }
  }

  for(i = 1; i < value; i++)
  {
    row_entry = gtk_grid_get_child_at(GTK_GRID(FloydParameters->grid_matrix),0,i);
    text1 = gtk_entry_get_text(GTK_ENTRY(row_entry));
    strcpy(graph->names[i], text1);
  }


  gtk_widget_set_sensitive(FloydParameters->btn_run_floyd, FALSE);
  gtk_widget_set_sensitive(FloydParameters->btn_cancel_size, FALSE);
  gtk_widget_set_sensitive(FloydParameters->btn_open_file_window, FALSE);
  gtk_widget_set_sensitive(FloydParameters->grid_matrix, FALSE);
  show_table_window(graph, FloydParameters);
  write_file(graph, graph->node_count);

  return 0;
}


int main(int argc, char * argv[])
{

  GtkBuilder *builder;
  GtkWidget *shortest_path_floyd_window;
  WidgetGroup *FloydParameters = malloc(sizeof(WidgetGroup));

  gtk_init(NULL, NULL);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Shortest_Paths/shortest_path_floyd.glade", NULL);

  shortest_path_floyd_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window_floyd"));
  FloydParameters->btn_accept_size = GTK_WIDGET(gtk_builder_get_object(builder, "btn_accept_size"));
  FloydParameters->btn_cancel_size = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel_size"));
  FloydParameters->btn_run_floyd = GTK_WIDGET(gtk_builder_get_object(builder, "btn_run_floyd"));
  FloydParameters->btn_open_file_window = GTK_WIDGET(gtk_builder_get_object(builder, "btn_open_file_window"));
  FloydParameters->graph_size_field = GTK_WIDGET(gtk_builder_get_object(builder, "graph_size_field"));
  FloydParameters->grid_matrix = GTK_WIDGET(gtk_builder_get_object(builder, "grid_matrix"));


  gtk_builder_connect_signals(builder, NULL);


  g_signal_connect (shortest_path_floyd_window, "destroy",G_CALLBACK (on_window_destroy), NULL);
  g_signal_connect (FloydParameters->btn_accept_size, "clicked", G_CALLBACK(create_matrix), FloydParameters);
  g_signal_connect (FloydParameters->btn_cancel_size, "clicked", G_CALLBACK(cancel_matrix), FloydParameters);
  g_signal_connect (FloydParameters->btn_run_floyd, "clicked", G_CALLBACK(plot_graph), FloydParameters);
  g_signal_connect (FloydParameters->btn_open_file_window, "clicked", G_CALLBACK(show_file_window), FloydParameters);

  g_object_unref(builder);


  gtk_widget_show(shortest_path_floyd_window);
  gtk_main();


  return 0;
}
