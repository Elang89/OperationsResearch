#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H


typedef struct FileData{
    float values[50][200];
    int plan_duration;
    int product_viability;
    int new_product_cost;
    int status;
} FileData;

char token_buffer[200];

FileData* file_data_new();
void buffer_char(int c);
void clear_buffer();
int write_to_file(char * file_name, float *maintenance_costs,
  float *resale_prices, int plan_duration, int product_viability,
  int new_product_cost);
FileData* read_from_file(char * file_name);
void file_data_destroy(FileData *file_data);

#endif
