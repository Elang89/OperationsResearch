#ifndef LOAD_MODULE_H
#define LOAD_MODULE_H
#include <gtk/gtk.h>

typedef struct{
  int node_count;
  int option;
  int status;
  double probabilities[90];
  char names[30][200];
} FileData;


char token_buffer[300];

void buffer_char(int c);
void clear_buffer();
FileData * new_file_data();
void write_to_file(int option, int node_count,
int * probabilities, char * file_name);
void read_from_file(FileData * file_data, char * file_name);

#endif
