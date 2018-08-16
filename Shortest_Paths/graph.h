#ifndef GRAPH_H
#define GRAPH_H

typedef struct Graph
{
  int node_count;
  int **adj_matrix;
  char names [20][20];
} Graph;

Graph* new_graph(int nodes);
void insert_edge(Graph* graph, int source, int dest, int weight);
void destroy_graph(Graph* graph);
void print_graph(Graph* graph, int matrix_size);
void write_file(Graph* graph, int matrix_size);
void print_node_names(Graph* graph, int graph_size);

#endif
