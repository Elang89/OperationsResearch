
#include <gtk/gtk.h>

void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

int main() {
    GtkBuilder      *builder;
    GtkWidget       *pendingW;

    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Pending/pending_window.glade", NULL);

    pendingW = GTK_WIDGET(gtk_builder_get_object(builder, "pending_window"));
    gtk_builder_connect_signals(builder, NULL);

    g_signal_connect (pendingW, "destroy",G_CALLBACK (on_window_destroy), NULL);

    g_object_unref(builder);

    gtk_widget_show(pendingW);
    gtk_main();

    return 0;
}
