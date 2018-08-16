#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#define INF 9999

Graph* new_graph(int nodes)
{
  int i;
  int j;
  Graph* graph = (Graph*) malloc(sizeof(Graph));
  graph->node_count = nodes;
  graph->adj_matrix = malloc(nodes * sizeof(int*));

  for(i = 0; i < nodes; i++)
  {
    graph->adj_matrix[i] = malloc(nodes * sizeof(int));
  }

  for(i = 0; i < nodes; i++)
  {
    for(j = 0; j < nodes; j++)
    {
      graph->adj_matrix[i][j] = 0;
    }

  }

  return graph;
}

void insert_edge(Graph* graph, int source, int dest, int weight)
{
  if(weight > -1)
  {
    graph->adj_matrix[dest][source] = weight;
  }
  else
  {
    graph->adj_matrix[dest][source] = INF;
  }

}

void print_graph(Graph* graph, int matrix_size)
{
  int i;
  int j;

  for(i = 0; i < matrix_size; i++)
  {
    for(j = 0; j < matrix_size; j++)
    {
      printf("%d ", graph->adj_matrix[i][j]);
    }
    printf("\n");
  }
}

void destroy_graph(Graph* graph)
{
  int i;

  for(i = 0; i < graph->node_count; i++)
  {
    free(graph->adj_matrix[i]);
  }

  free(graph->adj_matrix);
  free(graph);
}

void print_node_names(Graph* graph, int graph_size)
{
  int i;

  for(i = 1; i <= graph_size; i++ )
  {
    printf("Node: %s\n", graph->names[i]);
  }
}

void write_file(Graph* graph, int matrix_size)
{
  int i;
  int j;
  FILE *file_to_write;

  file_to_write = fopen("entryTable.txt", "w+");


  for(i = 0; i < matrix_size; i++)
  {
    for(j = 0; j < matrix_size; j++)
    {
      if(graph->adj_matrix[i][j] == INF)
      {
        fprintf(file_to_write, "%d ", -1);
      }
      else
      {
        fprintf(file_to_write, "%d ",graph->adj_matrix[i][j]);
      }
    }
    fprintf(file_to_write, "\n");
  }
  fclose(file_to_write);
}
