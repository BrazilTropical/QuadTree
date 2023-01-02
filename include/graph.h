#ifndef GRAPH_H_
#define GRAPH_H_

#include "vector.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct graph_node
{
  int self;
  int parent;

  vector* predecessors;
  vector* successors;
} graph_node;

typedef struct graph
{
  graph_node* nodes;
  int num_nodes;
} graph;

void graph_new_edge(graph* graph, int from, int to);

graph* graph_init(int num_nodes);

void destroy_graph(graph* graph, int num_nodes);

void print_graph(graph* graph, int num_nodes);

#endif // GRAPH_H_
