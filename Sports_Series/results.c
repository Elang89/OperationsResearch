#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>


GtkWidget* result_window;
GtkWidget* label_teamA_chance;
GtkWidget* label_teamB_chance;
GtkWidget* label_teamA_name;
GtkWidget* label_teamB_name;

void on_results_destroy(GtkWidget* window, gpointer data)
{
  GtkWidget* parent = (GtkWidget*)gtk_window_get_transient_for(GTK_WINDOW(window));
  gtk_widget_set_sensitive(parent, TRUE);
  gtk_widget_destroy(result_window);
}


void display_results(GtkWidget* parent, char * teamA_name, char * teamB_name,
  double probability)
{
 GtkBuilder *builder;
 char str1[50];
 char str2[50];

 double teamA_prob = probability * 100;
 double teamB_prob = (1 - probability) * 100;

 sprintf(str1, "%.2f",teamA_prob);
 sprintf(str2, "%.2f",teamB_prob);
 strcat(str1,"%");
 strcat(str2,"%");

 builder = gtk_builder_new();
 gtk_builder_add_from_file (builder, "Sports_Series/series.glade", NULL);

 result_window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));
 label_teamA_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_teamA_name"));
 label_teamB_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_teamB_name"));
 label_teamA_chance = GTK_WIDGET(gtk_builder_get_object(builder, "label_teamA_chance"));
 label_teamB_chance = GTK_WIDGET(gtk_builder_get_object(builder, "label_teamB_chance"));

 gtk_widget_set_sensitive(parent, FALSE);
 gtk_window_set_transient_for(GTK_WINDOW(result_window),GTK_WINDOW(parent));

 gtk_label_set_text(GTK_LABEL(label_teamA_name),teamA_name);
 gtk_label_set_text(GTK_LABEL(label_teamB_name),teamB_name);
 gtk_label_set_text(GTK_LABEL(label_teamA_chance),str1);
 gtk_label_set_text(GTK_LABEL(label_teamB_chance),str2);

 g_signal_connect(result_window, "destroy", G_CALLBACK(on_results_destroy), NULL);

 g_object_unref(builder);

 gtk_widget_show_all(result_window);
}
