#ifndef OPEN_FILE_H
#define OPEN_FILE_H
#include "structs.h"

typedef struct WidgetItems
{
  WidgetGroup *parent_object;
  GtkWidget *file_window;
  GtkWidget *btn_search_file;
  GtkWidget *btn_cancel_file_window;
  GtkWidget *fileChooser;
}WidgetItems;
char number[128];

void dialog_window();
void buffer_char(char * temp_char);
int open_search_file();
void show_file_window();

#endif
