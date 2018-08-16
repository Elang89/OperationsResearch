#ifndef SHORTEST_PATHS_H
#define SHORTEST_PATHS_H

void on_window_destroy(GtkWidget *window, gpointer data);
void on_node_entry_edited(GtkEntry *entry, gpointer data);
void create_matrix(GtkWidget *button, gpointer data);
void cancel_matrix(GtkWidget *button, gpointer data);
int plot_graph(GtkWidget *widget, gpointer data);
void openFile_window();
void load_user_file(char *filename);



#endif
