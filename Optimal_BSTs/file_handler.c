#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "file_handler.h"


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

FileData * new_file_data()
{
  FileData * file_data = (FileData*)calloc(200, sizeof(FileData));
	if(!file_data)
	{
		printf("Failed to allocate file data, exiting\n");
		exit(1);
	}
  return file_data;
}

void read_from_file(FileData * file_data, char * file_name)
{
  FILE * file = fopen(file_name, "r");
  int node_count = 0;
  int line_number = 1;
  int option = 0;
  double value = 0;
  char placeholder[90];
  char * str;
  char input_character;
  char temp_character;

  while((input_character = fgetc(file)) != EOF)
  {
    if(input_character == '\n')
    {
      line_number++;
    }
    else if (isspace(input_character))
    {
      continue;
    }
    else if(isdigit(input_character))
    {
      buffer_char(input_character);
      if(line_number == 1)
      {
				for(temp_character = fgetc(file); isdigit(temp_character);
        temp_character = fgetc(file))
        {
          buffer_char(temp_character);
        }
        ungetc(temp_character,file);;
        node_count = strtol(token_buffer, &str, 10);
        clear_buffer();
      }
      if(line_number == 2)
      {
        for(temp_character = fgetc(file); isdigit(temp_character);
        temp_character = fgetc(file))
        {
          buffer_char(temp_character);
        }
        ungetc(temp_character,file);
        option = strtol(token_buffer, &str, 10);
        clear_buffer();
      }
      else if(line_number >= 3)
      {
        for(temp_character = fgetc(file); isdigit(temp_character) ||
        temp_character == ','; temp_character = fgetc(file))
        {
          buffer_char(temp_character);
        }
        ungetc(temp_character,file);
        value = strtof(token_buffer, &str);
        file_data->probabilities[line_number - 3] = value;
        clear_buffer();
      }
    }
    else if(isalpha(input_character))
    {
      buffer_char(input_character);
      for(temp_character = fgetc(file); isalnum(temp_character) ||
				temp_character == '_';
        temp_character = fgetc(file))
      {
        buffer_char(temp_character);
      }
      ungetc(temp_character,file);
      strncpy(placeholder,token_buffer,50);
      strncpy(file_data->names[line_number - 3],placeholder,50);
      clear_buffer();
    }
    else
    {
			file_data->status = 1;
      return;
    }
  }
	file_data->status = 0;
  file_data->option = option;
  file_data->node_count = node_count;

  fclose(file);
}
