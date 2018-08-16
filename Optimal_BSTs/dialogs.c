#include <gtk/gtk.h>
#include "dialogs.h"


void close_dialog(GtkWidget* dialog, gpointer data)
{
  GtkWindow* parent = gtk_window_get_transient_for(GTK_WINDOW(dialog));
  gtk_widget_set_sensitive(GTK_WIDGET(parent), TRUE);
  gtk_widget_destroy(dialog);
}

void error_dialog(GtkWidget *parent, char * message)
{
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *content_area;

  dialog = gtk_dialog_new();
  label = gtk_label_new(message);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(parent));
  gtk_widget_set_sensitive(parent, FALSE);

  g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(close_dialog),NULL);

  gtk_dialog_add_button(GTK_DIALOG(dialog),"OK", GTK_RESPONSE_NONE);
  gtk_container_add(GTK_CONTAINER(content_area),label);
  gtk_widget_show_all(dialog);
}
