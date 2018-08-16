#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>


void on_window_destroy(GtkWidget *window, gpointer data)
{
  gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *main_window;


    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Main_Window/IO_Project0_v1.glade", NULL);

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_builder_connect_signals(builder, NULL);

    g_signal_connect (main_window, "destroy",G_CALLBACK (on_window_destroy), NULL);

    g_object_unref(builder);


    gtk_widget_show(main_window);
    gtk_main();

    return 0;
}

void create_pending_window(GtkWidget *button, gpointer window)
{

  system("./PendingW &");

}

void create_shortest_paths_window(GtkWidget *button)
{

  system("./Floyd &");
}

void create_knapsack_window(GtkWidget *button, gpointer window)
{
  system("./Knapsack &");
}

void create_machineM_window(GtkWidget *button, gpointer window)
{
  system("./Equipment &");
}

void create_series_window(GtkWidget *button, gpointer window)
{
  system("./Series &");
}

void create_BST_window(GtkWidget *button, gpointer window)
{
  system("./BST &");
}

void create_matmult_window(GtkWidget *button, gpointer window)
{
  system("./MatMult &");
}
