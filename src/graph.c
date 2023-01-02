#include <stdio.h>
#include "graph.h"

void graph_new_edge(graph* graph, int from, int to)
{
  vector_push_back(graph->nodes[from].successors, to);
  vector_push_back(graph->nodes[to  ].successors, from);
}

graph* graph_init(int num_nodes)
{
  graph* graph = malloc(sizeof(graph));
  graph->nodes      = malloc(sizeof(graph_node) * num_nodes);
  for ( int i = 0; i < num_nodes; ++i )
  {
    graph->nodes[i].self         = i;
    graph->nodes[i].parent       = -1;
    graph->nodes[i].successors   = vector_init( );
    graph->nodes[i].predecessors = vector_init( );
  }

  return graph;
}

void destroy_graph(graph* graph, int num_nodes)
{
  for ( int i = 0; i < num_nodes; ++i )
  {
    vector_free(graph->nodes[i].predecessors);
    vector_free(graph->nodes[i].successors);
  }

  free(graph->nodes);
  free(graph);
}


void print_graph(graph* graph, int num_nodes)
{
  printf("------------------\n");
  printf("Neighbors \n");
  for ( int i = 0; i < num_nodes; ++i )
  {
    int num_neighbors = graph->nodes[i].successors->size;

    if ( num_neighbors ) printf("[Rank: %d] Neighbors: ", i + 1);
    for ( int j = 0; j < num_neighbors; ++j )
    {
      printf("%d ", graph->nodes[i].successors->array[j] + 1);
    }

    if ( num_neighbors ) printf("\n");
  }
  printf("------------------\n");
}
