#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "file_handler.h"


FileData* file_data_new()
{
  FileData* file_data = calloc(400,sizeof(FileData));
  return file_data;
}

void clear_buffer()
{
	int i;
	for(i = 0; i < sizeof(token_buffer); i++)
	{
		token_buffer[i] = 0;
	}
}

void buffer_char(int c)
{
	int i;
	for(i = 0; i < sizeof(token_buffer); i++)
	{
    if(token_buffer[i] == 0)
    {
		  token_buffer[i] = c;
      break;
    }
	}
}

int write_to_file(char * file_name, double probH_A, double probH_B,
  double probA_A, double probA_B, char * teamA_name, char * teamB_name,
  int game_number, int * series_arrangement)
{
  int i;
  FILE * file;
  strcat(file_name, ".txt");
  file = fopen(file_name, "w");

  if(file == NULL)
  {
      return 1;
  }

  fprintf(file, "%.2f\n", probH_A);
  fprintf(file, "%.2f\n", probH_B);
  fprintf(file, "%.2f\n", probA_A);
  fprintf(file, "%.2f\n", probA_B);
  fprintf(file, "%s\n", teamA_name);
  fprintf(file, "%s\n", teamB_name);
  fprintf(file, "%d\n", game_number);

  for(i = 0; i < game_number; i++)
  {
    fprintf(file,"%d ",series_arrangement[i]);
  }
  fprintf(file, "\n");
  fclose(file);
  return 0;
}

FileData* read_from_file(char * file_name)
{
  FileData *file_data = file_data_new();
  FILE * file;
  file = fopen(file_name, "r");
  int line_number = 0;
  int game_number = 0;
  int i = 0;
  int size = 1;
  double value;
  double probabilities[4];
  char names[10][30];
  char * str;
  char array_number;
  char placeholder[30];
  char input_character;
  char temp_character;
  int arrangement[1000];

  while((input_character = fgetc(file)) != EOF)
  {
    if(input_character == '\n')
    {
      line_number++;
    }
    else if(isspace(input_character))
    {
      continue;
    }
    else if(isdigit(input_character))
    {
      buffer_char(input_character);
      if(line_number < 4)
      {
        for(temp_character = fgetc(file); isdigit(temp_character) ||
        temp_character == ','; temp_character = fgetc(file))
        {
          buffer_char(temp_character);
        }
        ungetc(temp_character,file);
        value = strtof(token_buffer, &str);
        probabilities[line_number] = value;
        clear_buffer();
      }
      else if(line_number == 6)
      {
        for(temp_character = fgetc(file); isdigit(temp_character);
          temp_character = fgetc(file))
        {
          buffer_char(temp_character);
        }
        ungetc(temp_character,file);
        value = strtol(token_buffer, &str, 10);
        game_number = value;
        clear_buffer();
      }
      else
      {
        for(temp_character = fgetc(file); isspace(temp_character) ||
          isdigit(temp_character); temp_character = fgetc(file))
        {
          if(isdigit(temp_character))
          {
            buffer_char(temp_character);
            size++;
          }
        }
        for(i = 0; i < size; i++)
        {
          array_number = token_buffer[i];
          value = strtol(&array_number,&str,10);
          arrangement[i] = value;
        }
      }
    }
    else if(isalpha(input_character))
    {
      buffer_char(input_character);
      for(temp_character = fgetc(file); isalnum(temp_character) ||
				temp_character == '_' || isspace(temp_character) && temp_character != '\n';
        temp_character = fgetc(file))
      {
        buffer_char(temp_character);
      }
      ungetc(temp_character,file);
      strncpy(placeholder,token_buffer,30);
      strncpy(names[line_number-4+1], placeholder,30);
      clear_buffer();
    }
    else
    {
      file_data->status = 1;
      return file_data;
    }
    clear_buffer();
  }
  file_data->status = 0;
  file_data->probH_A = probabilities[0];
  file_data->probH_B = probabilities[1];
  file_data->probA_A = probabilities[2];
  file_data->probA_B = probabilities[3];
  strncpy(file_data->teamA_name,names[1],30);
  strncpy(file_data->teamB_name,names[2],30);
  file_data->game_number = game_number;
  file_data->series_arrangement = calloc(file_data->game_number, sizeof(int));

  for(i = 0; i < file_data->game_number; i++)
  {
    file_data->series_arrangement[i] = arrangement[i];
  }


  return file_data;

}


void file_data_destroy(FileData * file_data)
{
  free(file_data);
}
