#ifndef FILE_HANDLER_MODEL_H
#define FILE_HANDLER_MODEL_H


typedef struct FileData{
  int game_number;
  int best_of_games;
  int status;
  int * series_arrangement;
  double probH_A;
  double probH_B;
  double probA_A;
  double probA_B;
  char teamA_name[30];
  char teamB_name[30];
} FileData;

char token_buffer[400];

FileData* file_data_new();
void buffer_char(int c);
void clear_buffer();
int write_to_file(char * file_name, double probH_A, double probH_B,
  double probA_A, double probA_B, char * teamA_name, char * teamB_name,
  int game_number, int * series_arrangement);
FileData* read_from_file(char * file_name);
void file_data_destroy(FileData *file_data);

#endif
