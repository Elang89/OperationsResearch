#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dialogs.h"
#include "file_handler.h"
#include "series_model.h"
#include "results.h"
#define MAX_PROBABILITY 1

typedef struct SeriesData {
  int game_number;
  int best_of_games;
  double probH_A;
  double probH_B;
  double probA_A;
  double probA_B;
  int series_arrangement;
  char teamA_name[30];
  char teamB_name[30];
  double result;
} SeriesData;

GtkWidget *main_window_series;
GtkWidget *btn_accept;
GtkWidget *btn_cancel;
GtkWidget *filechooser_window;
GtkWidget *btn_load_file;
GtkWidget *btn_cancel_file;
GtkWidget *txt_teamA;
GtkWidget *txt_teamB;
GtkWidget *spb_game_number;
GtkWidget *spb_probH_A_value;
GtkWidget *spb_probH_B_value;
GtkWidget *spb_probA_A_value;
GtkWidget *spb_probA_B_value;
GtkWidget *file_name_field;
GtkWidget *btn_run;
GtkWidget *btn_read_file;
GtkWidget *btn_reset;
GtkWidget *file_name_field;
GtkWidget *btn_probabilities;
GtkWidget *btn_create_file;
GtkWidget *btn_exit;
GtkWidget *grid_results;
GtkWidget* game_field_options;
GtkWidget *scrolledwindow_results;
GtkWidget *viewport_results;
GtkWidget *scrolledwindow_games;
GtkWidget *viewport_games;
SeriesData *series_data;


//This destroys window, btn_exit calls this function
void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

//This resets everything in the window, btn_reset calls this
void reset_window(GtkWidget *button, gpointer data)
{
  gtk_widget_destroy(grid_results);
  gtk_widget_destroy(game_field_options);

  gtk_entry_set_text(GTK_ENTRY(file_name_field),"");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_create_file), FALSE);;

  gtk_widget_set_sensitive(btn_reset, FALSE);
  gtk_widget_set_sensitive(btn_probabilities, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(btn_read_file, TRUE);
  gtk_widget_set_sensitive(spb_game_number, TRUE);
  gtk_widget_set_sensitive(txt_teamA, TRUE);
  gtk_widget_set_sensitive(txt_teamB, TRUE);
  gtk_widget_set_sensitive(spb_probH_A_value, TRUE);
  gtk_widget_set_sensitive(spb_probH_B_value, TRUE);
  gtk_widget_set_sensitive(spb_probA_A_value, TRUE);
  gtk_widget_set_sensitive(spb_probA_B_value, TRUE);
  gtk_widget_set_sensitive(btn_accept, TRUE);

  gtk_entry_set_text(GTK_ENTRY(txt_teamA), "Team A");
  gtk_entry_set_text(GTK_ENTRY(txt_teamB), "Team B");

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_game_number),1);
}

/*Create grid and plot results*/
void create_result_grid(int best_of_games,
  double result_matrix[best_of_games+1][best_of_games+1])
{
  int i = 0;
  int j = 0;
  char str[20];
  double probability_value = 0;
  GtkWidget *entry;
  grid_results = gtk_grid_new();

  for(i = 1; i <= series_data->best_of_games + 1; i++)
  {
    sprintf(str, "%d", i - 1);
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry),str);
    g_object_set(entry, "editable", TRUE, NULL);
    gtk_grid_attach(GTK_GRID(grid_results), entry,i,0,1,1);
  }

  for(i = 1; i <= series_data->best_of_games + 1; i++)
  {
    sprintf(str, "%d", i - 1);
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry),str);
    g_object_set(entry, "editable", TRUE, NULL);
    gtk_grid_attach(GTK_GRID(grid_results), entry,0,i,1,1);
  }
  entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(entry),"0");
  gtk_grid_attach(GTK_GRID(grid_results),entry,1,1,1,1);

  for(i = 1; i <= series_data->best_of_games + 1 ; i++)
  {
    for(j = 1; j <= series_data->best_of_games + 1; j++)
    {
      probability_value = result_matrix[j-1][i-1];
      sprintf(str, "%.4f", probability_value);
      entry = gtk_entry_new();
      gtk_entry_set_text(GTK_ENTRY(entry),str);
      gtk_grid_attach(GTK_GRID(grid_results),entry,i,j,1,1);
      g_object_set(entry,"editable", FALSE, NULL);
    }
  }
  series_data->result = result_matrix[series_data->best_of_games][series_data->best_of_games];

  gtk_grid_set_column_spacing(GTK_GRID(grid_results),5);
  gtk_grid_set_row_spacing(GTK_GRID(grid_results),5);
  gtk_container_add(GTK_CONTAINER(viewport_results),grid_results);
  gtk_widget_show_all(scrolledwindow_results);

}

/*Requests an answer from series model, sends the series arrangement,
receives probabilities as a response, then it calls creation of result grid */
void request_probabilities()
{
  int i = 1;
  double result_matrix[series_data->best_of_games+1][series_data->best_of_games+1];
  int series_arrangement[series_data->best_of_games];
  char file_name[50];
  int status_code;
  GList * children = gtk_container_get_children(GTK_CONTAINER(game_field_options));
  GList * iterator;
  GtkWidget* toggle_button;


  iterator = children;
  for(i = 0; i < series_data->game_number; i++)
  {

    toggle_button = (GtkWidget*) iterator->data;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button)))
    {
      series_arrangement[i] = 1;
    }
    else
    {
      series_arrangement[i] = 0;
    }
    iterator = g_list_next(iterator);
  }
  calculate_results(series_arrangement, series_data->best_of_games, result_matrix);
  create_result_grid(series_data->best_of_games, result_matrix);

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn_create_file)) == TRUE)
    {
      strncpy(file_name,gtk_entry_get_text(GTK_ENTRY(file_name_field)),50);

      status_code = write_to_file(file_name, series_data->probH_A,
        series_data->probH_B, series_data->probA_A, series_data->probA_B,
        series_data->teamA_name, series_data->teamB_name,
        series_data->game_number,series_arrangement);

      if(status_code == 1)
      {
        error_dialog(main_window_series, "File name is invalid.");
      }
    }
}

/*Checks if file field has a valid name, if not process will halt,
and display an error message*/
void verify_file_field(GtkWidget *button, gpointer data)
{
  unsigned int length = gtk_entry_get_text_length(GTK_ENTRY(file_name_field));

  if(length == 0 &&
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn_create_file)) == TRUE)
  {
    error_dialog(main_window_series, "File name is empty.");
    return;
  }

  gtk_widget_set_sensitive(file_name_field, FALSE);
  gtk_widget_set_sensitive(btn_create_file, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_widget_set_sensitive(btn_probabilities, TRUE);
  gtk_widget_set_sensitive(btn_read_file, FALSE);
  gtk_widget_set_sensitive(btn_reset, TRUE);
  gtk_widget_set_sensitive(btn_cancel, FALSE);

  set_probabilities(series_data->probH_A, series_data->probH_B,
    series_data->probA_A, series_data->probA_B);
  request_probabilities();
}

//Create field to decide which games will have a home field advantage
void create_game_field(int number_of_fields)
{
  int  i = 0;
  char game[30] = "Game ";
  char str[20];
  game_field_options = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget* check_button;

  for(i = 1; i <= number_of_fields; i++)
  {
    sprintf(str,"%d",i);
    strcat(game, str);
    check_button = gtk_check_button_new_with_label(game);
    gtk_container_add(GTK_CONTAINER(game_field_options), check_button);
    game[0] = '\0';
    str[0] = '\0';
    strncpy(game,"Game ",10);
  }
  gtk_container_add(GTK_CONTAINER(viewport_games),game_field_options);
  gtk_widget_show_all(scrolledwindow_games);
}

/*Calculates the opposing parameter's probability, this is set to listen
to either field, if one changes so does the other*/
void calculate_opposite_probability(GtkWidget* spb_button, gpointer data)
{
  GtkWidget* spb_opposite_button = (GtkWidget*)data;
  double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_button));
  double opposite_value = MAX_PROBABILITY - value;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_opposite_button),opposite_value);
}

/*Check if team names are empty*/
int verify_team_names()
{
  int teamA_length = gtk_entry_get_text_length(GTK_ENTRY(txt_teamA));
  int teamB_length = gtk_entry_get_text_length(GTK_ENTRY(txt_teamB));

  if(teamA_length == 0 || teamB_length == 0)
  {
    error_dialog(main_window_series,"There is an empty team name, team must be named");
    return 1;
  }
  return 0;
}

/*Set data to struct*/
void set_series_data()
{
  int game_number = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_game_number));
  double probH_A = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_probH_A_value));
  double probH_B = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_probH_B_value));
  double probA_A = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_probA_A_value));
  double probA_B = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spb_probA_B_value));
  char teamA_name[30];
  char teamB_name[30];
  int lengthA = gtk_entry_get_text_length(GTK_ENTRY(txt_teamA));
  int lengthB = gtk_entry_get_text_length(GTK_ENTRY(txt_teamB));

  strncpy(teamA_name,gtk_entry_get_text(GTK_ENTRY(txt_teamA)),lengthA);
  strncpy(teamB_name,gtk_entry_get_text(GTK_ENTRY(txt_teamB)),lengthB);

  series_data->game_number = game_number;
  series_data->best_of_games = (series_data->game_number + 1)/2;
  series_data->probH_A = probH_A;
  series_data->probH_B = probH_B;
  series_data->probA_A = probA_A;
  series_data->probA_B = probA_B;
  strncpy(series_data->teamA_name,gtk_entry_get_text(GTK_ENTRY(txt_teamA)),lengthA);
  strncpy(series_data->teamB_name,gtk_entry_get_text(GTK_ENTRY(txt_teamB)),lengthB);
}

/*Disable buttons, call team name input verification function and finally
call result grid creation function */
void accept(GtkWidget *button, gpointer data)
{
  if(verify_team_names())
    return;
  set_series_data();

  gtk_widget_set_sensitive(btn_cancel, TRUE);
  gtk_widget_set_sensitive(btn_run,TRUE);
  gtk_widget_set_sensitive(btn_create_file,TRUE);
  gtk_widget_set_sensitive(file_name_field, TRUE);

  gtk_widget_set_sensitive(spb_probH_A_value, FALSE);
  gtk_widget_set_sensitive(spb_probH_B_value, FALSE);
  gtk_widget_set_sensitive(spb_probA_A_value,FALSE);
  gtk_widget_set_sensitive(spb_probA_B_value,FALSE);
  gtk_widget_set_sensitive(txt_teamA,FALSE);
  gtk_widget_set_sensitive(txt_teamB,FALSE);
  gtk_widget_set_sensitive(spb_game_number,FALSE);
  gtk_widget_set_sensitive(btn_load_file,FALSE);
  gtk_widget_set_sensitive(btn_accept,FALSE);

  create_game_field(series_data->game_number);

}

/*Enable buttons disabled by accept, destroy grid*/
void cancel(GtkWidget* button, gpointer data)
{
  gtk_widget_set_sensitive(txt_teamA,TRUE);
  gtk_widget_set_sensitive(txt_teamB,TRUE);
  gtk_widget_set_sensitive(spb_probH_A_value,TRUE);
  gtk_widget_set_sensitive(spb_probH_B_value,TRUE);
  gtk_widget_set_sensitive(spb_probA_A_value,TRUE);
  gtk_widget_set_sensitive(spb_probA_B_value,TRUE);
  gtk_widget_set_sensitive(txt_teamA,TRUE);
  gtk_widget_set_sensitive(txt_teamB,TRUE);
  gtk_widget_set_sensitive(spb_game_number,TRUE);
  gtk_widget_set_sensitive(btn_accept,TRUE);

  gtk_widget_set_sensitive(file_name_field, FALSE);
  gtk_widget_set_sensitive(btn_cancel, FALSE);
  gtk_widget_set_sensitive(btn_run, FALSE);
  gtk_entry_set_text(GTK_ENTRY(file_name_field),"");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_create_file),FALSE);

  gtk_widget_destroy(game_field_options);
}

/*Load file data into view*/
void load_view_data(FileData* file_data)
{
  int i = 0;
  GtkWidget* check_button;
  GList * children;
  GList * iterator;

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_probH_A_value), file_data->probH_A);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_probH_B_value), file_data->probH_B);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_probA_A_value), file_data->probA_A);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_probA_B_value), file_data->probA_B);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spb_game_number), file_data->game_number);
  gtk_entry_set_text(GTK_ENTRY(txt_teamA), file_data->teamA_name);
  gtk_entry_set_text(GTK_ENTRY(txt_teamB), file_data->teamB_name);

  gtk_widget_set_sensitive(btn_cancel, TRUE);
  gtk_widget_set_sensitive(btn_run,TRUE);
  gtk_widget_set_sensitive(btn_create_file,TRUE);
  gtk_widget_set_sensitive(file_name_field, TRUE);

  gtk_widget_set_sensitive(spb_probH_A_value, FALSE);
  gtk_widget_set_sensitive(spb_probH_B_value, FALSE);
  gtk_widget_set_sensitive(spb_probA_A_value,FALSE);
  gtk_widget_set_sensitive(spb_probA_B_value,FALSE);
  gtk_widget_set_sensitive(txt_teamA,FALSE);
  gtk_widget_set_sensitive(txt_teamB,FALSE);
  gtk_widget_set_sensitive(spb_game_number,FALSE);
  gtk_widget_set_sensitive(btn_load_file,FALSE);
  gtk_widget_set_sensitive(btn_accept,FALSE);

  create_game_field(file_data->game_number);
  set_series_data();


  children = gtk_container_get_children(GTK_CONTAINER(game_field_options));
  iterator = children;
  for(i = 0; i < file_data->game_number; i++)
  {
    if(file_data->series_arrangement[i] == 1)
    {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(iterator->data),TRUE);
    }
    iterator = g_list_next(iterator);
  }

}

/*Display final results*/
void open_results()
{
  display_results(main_window_series, series_data->teamA_name,
    series_data->teamB_name, series_data->result);
}

/*Close file chooser window*/
void close_filechooser_window(GtkWidget* button, gpointer data)
{
  gtk_widget_set_sensitive(main_window_series, TRUE);
  gtk_widget_destroy(filechooser_window);
}

/*Load file name into file handler*/
int load_file_name(GtkWidget* button, gpointer data)
{
  char * file_name;
  FileData *file_data;
  file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser_window));
  file_data = read_from_file(file_name);

  if(file_data->status == 1)
  {
    error_dialog(filechooser_window, "Invalid file, please choose a valid file. Files can only contain names and numbers.");
    return 0;
  }
  load_view_data(file_data);
  close_filechooser_window(NULL, NULL);

  return 1;
}

void show_filechooser()
{
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "Sports_Series/series.glade", NULL);

  filechooser_window = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_window"));
  btn_load_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_load_file"));
  btn_cancel_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel_file"));

  gtk_window_set_transient_for(GTK_WINDOW(filechooser_window), GTK_WINDOW(main_window_series));

  g_signal_connect(btn_load_file, "clicked", G_CALLBACK(load_file_name), NULL);
  g_signal_connect(btn_cancel_file, "clicked", G_CALLBACK(close_filechooser_window), NULL);
  g_signal_connect(filechooser_window, "destroy", G_CALLBACK(close_filechooser_window), NULL);
  gtk_widget_set_sensitive(main_window_series, FALSE);

  g_object_unref(builder);
  gtk_widget_show(filechooser_window);
}

/*Assign objects to variables and display main window*/
void assign_and_display_objects()
{
   GtkBuilder *builder;

   builder = gtk_builder_new();
   gtk_builder_add_from_file (builder, "Sports_Series/series.glade", NULL);

   main_window_series = GTK_WIDGET(gtk_builder_get_object(builder, "main_window_series"));
   btn_accept = GTK_WIDGET(gtk_builder_get_object(builder, "btn_accept"));
   btn_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_cancel"));
   txt_teamA = GTK_WIDGET(gtk_builder_get_object(builder, "txt_teamA"));
   txt_teamB = GTK_WIDGET(gtk_builder_get_object(builder, "txt_teamB"));
   spb_probH_A_value = GTK_WIDGET(gtk_builder_get_object(builder, "spb_probH_A_value"));
   spb_probH_B_value = GTK_WIDGET(gtk_builder_get_object(builder, "spb_probH_B_value"));
   spb_probA_A_value = GTK_WIDGET(gtk_builder_get_object(builder, "spb_probA_A_value"));
   spb_probA_B_value = GTK_WIDGET(gtk_builder_get_object(builder, "spb_probA_B_value"));
   spb_game_number = GTK_WIDGET(gtk_builder_get_object(builder, "spb_game_number"));
   scrolledwindow_results = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_results"));
   viewport_results = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_results"));
   scrolledwindow_games = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_games"));
   viewport_games = GTK_WIDGET(gtk_builder_get_object(builder, "viewport_games"));
   file_name_field = GTK_WIDGET(gtk_builder_get_object(builder, "file_name_field"));
   btn_read_file = GTK_WIDGET(gtk_builder_get_object(builder,"btn_read_file"));
   btn_run = GTK_WIDGET(gtk_builder_get_object(builder, "btn_run"));
   btn_reset = GTK_WIDGET(gtk_builder_get_object(builder, "btn_reset"));
   btn_probabilities = GTK_WIDGET(gtk_builder_get_object(builder, "btn_probabilities"));
   btn_create_file = GTK_WIDGET(gtk_builder_get_object(builder, "btn_create_file"));
   btn_exit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_exit"));

   g_object_unref(builder);

   g_signal_connect(main_window_series, "destroy", G_CALLBACK(on_window_destroy),NULL);
   g_signal_connect(btn_accept, "clicked", G_CALLBACK(accept), NULL);
   g_signal_connect(btn_cancel, "clicked", G_CALLBACK(cancel), NULL);
   g_signal_connect(spb_probH_A_value, "value_changed",
    G_CALLBACK(calculate_opposite_probability), (gpointer*)spb_probA_B_value);
   g_signal_connect(spb_probH_B_value, "value_changed",
    G_CALLBACK(calculate_opposite_probability), (gpointer*)spb_probA_A_value);
   g_signal_connect(spb_probA_A_value, "value_changed",
    G_CALLBACK(calculate_opposite_probability), (gpointer*)spb_probH_B_value);
   g_signal_connect(spb_probA_B_value, "value_changed",
    G_CALLBACK(calculate_opposite_probability), (gpointer*)spb_probH_A_value);

   g_signal_connect(btn_probabilities, "clicked", G_CALLBACK(open_results), NULL);
   g_signal_connect(btn_read_file, "clicked", G_CALLBACK(show_filechooser),NULL);
   g_signal_connect(btn_reset, "clicked", G_CALLBACK(reset_window), NULL);
   g_signal_connect(btn_run, "clicked", G_CALLBACK(verify_file_field),NULL);
   g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
   series_data = (SeriesData*)calloc(80,sizeof(SeriesData));
   if(!series_data)
   {
     error_dialog(main_window_series, "Failed to allocate memory for series_data, exiting program");
     exit(1);
   }

   gtk_widget_show(main_window_series);
}
