#ifndef LOAD_MODULE_H
#define LOAD_MODULE_H
#include <gtk/gtk.h>
#include "types.h"

void buffer_char(int c);
void clear_buffer();
FileData * new_file_data();
void read_from_file(FileData * file_data, const char * file_name);

#endif
