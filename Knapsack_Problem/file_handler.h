#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H


typedef struct FileData{
    char names[200][20];
    int values[200][200];
    int object_number;
    int capacity;
    int status;
} FileData;

char token_buffer[200];

FileData* file_data_new();
void buffer_char(int c);
void clear_buffer();
int write_to_file(char * file_name, char **names , int *values,
int *weights, int *amounts, int line_number, int capacity);
FileData* read_from_file(char * file_name);
void file_data_destroy(FileData *file_data);

#endif
