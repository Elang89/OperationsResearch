#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "file_handler.h"


FileData* file_data_new()
{
  FileData* file_data = malloc(sizeof(FileData));
  int i;
  int j;

  for(i = 0; i < 3; i++)
  {
    for(j = 0; j < 200; j++)
    {
      file_data->values[i][j] = 0;
    }
  }


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

int write_to_file(char * file_name, char **names , int *values,
  int *weights, int *amounts, int line_number, int capacity)
{
  int i;
  FILE * file;
  strcat(file_name, ".txt");
  file = fopen(file_name, "w");

  if(file == NULL)
  {
      return 0;
  }

  fprintf(file, "%d\n", capacity);
  for(i = 1; i <= line_number; i++)
  {
    fprintf(file, "%s %d %d %d", names[i], values[i], weights[i], amounts[i]);
    fprintf(file, "\n");
  }
  fclose(file);
  return 1;
}

FileData* read_from_file(char * file_name)
{
  FileData *file_data = file_data_new();
  FILE * file;
  file = fopen(file_name, "r");
  int row = 0;
  int column = 0;
  int name_index = 0;
  int size;
  int value;
  int input_character;
  int temp_character;

  input_character = fgetc(file);

  if(isdigit(input_character))
  {
    buffer_char(input_character);

    for (temp_character = fgetc(file); isdigit(temp_character)
    ;temp_character = fgetc(file))
    {
      buffer_char(temp_character);
    }
    size = atoi(token_buffer);
    clear_buffer();
  }
  else
  {
    file_data->status = 0;
    return file_data;
  }

  while((input_character = fgetc(file)) != EOF)
  {
    if(input_character == '\n')
    {
        row++;
        column = 0;
    }
    else if(isspace(input_character))
    {
      continue;
    }
    else if(isalpha(input_character))
    {
      buffer_char(input_character);

      for (temp_character = fgetc(file); isalpha(temp_character) ||
      temp_character == '_' || isdigit(temp_character); temp_character = fgetc(file))
			{
        buffer_char(temp_character);
			}

      ungetc(temp_character,file);
      strcpy(file_data->names[name_index],token_buffer);
      clear_buffer();
      name_index++;
    }
    else if(isdigit(input_character))
    {
      buffer_char(input_character);
      for (temp_character = fgetc(file); isdigit(temp_character);
      temp_character = fgetc(file))
			{
        buffer_char(temp_character);
			}
      ungetc(temp_character,file);
      value =  atoi(token_buffer);
      file_data->values[row][column] = value;
      clear_buffer();
      column++;
    }
    else
    {
      file_data->status = 0;
      return file_data;
      break;
    }
  }

  file_data->object_number = row;
  file_data->capacity = size;
  file_data->status = 1;

  return file_data;
}

void file_data_destroy(FileData * file_data)
{
  free(file_data);
}
