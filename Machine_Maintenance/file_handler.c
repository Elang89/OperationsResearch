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

  for(i = 0; i < 2; i++)
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

int write_to_file(char * file_name, float *maintenance_costs,
  float *resale_prices, int plan_duration, int product_viability,
  int new_product_cost)
{
  int i;
  FILE * file;
  strcat(file_name, ".txt");
  file = fopen(file_name, "w");

  if(file == NULL)
  {
      return 0;
  }

  fprintf(file, "%d ", plan_duration);
  fprintf(file, "%d ", product_viability);
  fprintf(file, "%d\n", new_product_cost);
  for(i = 1; i <= product_viability; i++)
  {
    fprintf(file, "%.2f %.2f", maintenance_costs[i], resale_prices[i]);
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
  int counter = 0;
  int size;
  int value;
  int input_character;
  int temp_character;

  while(counter < 3)
  {
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
    }
    else if(isspace(input_character))
    {
      continue;
    }
    else
    {
      file_data->status = 0;
      return file_data;
    }
    if(counter == 0)
    {
      file_data->plan_duration = size;
    }
    else if(counter == 1)
    {
      file_data->product_viability = size;
    }
    else
    {
      file_data->new_product_cost = size;
    }
    counter++;
    clear_buffer();
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
    else if(isdigit(input_character))
    {
      buffer_char(input_character);
      for (temp_character = fgetc(file); isdigit(temp_character) ||
      temp_character == ','; temp_character = fgetc(file))
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
  fclose(file);
  file_data->status = 1;

  return file_data;
}

void file_data_destroy(FileData * file_data)
{
  free(file_data);
}
