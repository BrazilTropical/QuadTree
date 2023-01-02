//
// Created by sk on 11/11/22.
//

#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "partitions.h"
#include "graph.h"
#include "quadtree.h"

#define INT_ROUND(X) ((int) round(X))
#define MAX(X, Y) (X > Y ? X : Y)
#define MIN(X, Y) (X < Y ? X : Y)
#define MAX_INT 1e4
#define DEBUG 0

void print_node(node* node)
{
  if(!node)
    return;

  print_node(node->children[0]);
  print_node(node->children[1]);
  print_node(node->children[2]);
  print_node(node->children[3]);

  if(node->is_terminal)
  {
    printf("Node: depth: %d is_terminal: %d tl[%d, %d] br[%d, %d]\n",
           node->depth,
           node->is_terminal,
           node->top_left_ix_x,
           node->top_left_ix_y,
           node->bottom_right_ix_x,
           node->bottom_right_ix_y);
  }

}

void print_tree(quad_tree* tree)
{
  print_node(tree->root);
}

node* split(node* Node)
{
  Node->is_terminal = false;

  int tl_x = Node->top_left_ix_x;
  int tl_y = Node->top_left_ix_y;

  int br_x = Node->bottom_right_ix_x;
  int br_y = Node->bottom_right_ix_y;

  int midpoint_x = INT_ROUND(( Node->top_left_ix_x + Node->bottom_right_ix_x ) / 2.0);
  int midpoint_y = INT_ROUND(( Node->top_left_ix_y + Node->bottom_right_ix_y ) / 2.0);

  Node->children[0] = new_node(Node,
                               NW,
                               Node->depth + 1,
                               Node->top_left_ix_x,
                               Node->top_left_ix_y,
                               INT_ROUND((Node->bottom_right_ix_x + Node->top_left_ix_x ) / 2.0),
                               INT_ROUND((Node->bottom_right_ix_y + Node->top_left_ix_y ) / 2.0));


  Node->children[1] = new_node(Node,
                               NE,
                               Node->depth + 1,
                               midpoint_x,
                               tl_y,
                               br_x,
                               midpoint_y);

  Node->children[2] = new_node(Node,
                               SE,
                               Node->depth + 1,
                               INT_ROUND((Node->top_left_ix_x + Node->bottom_right_ix_x) / 2.0),
                               INT_ROUND((Node->top_left_ix_y + Node->bottom_right_ix_y) / 2.0),
                               Node->bottom_right_ix_x,
                               Node->bottom_right_ix_y);


  Node->children[3] = new_node(Node,
                               SW,
                               Node->depth + 1,
                               tl_x,
                               midpoint_y,
                               midpoint_x,
                               br_y);

  return Node;
}

quad_tree* split_tree(quad_tree* qt,
                      int m_size,
                      int matrix[m_size][m_size],
                      int epsilon)
{
  Queue nodes;

  queueInit(&nodes, sizeof(node*));

  enqueue(&nodes, &qt->root);

  int res[m_size][m_size];

  int zone = 1;

  while(getQueueSize(&nodes) != 0)
  {
    node* top_queue;
    dequeue(&nodes, &top_queue);
    int local_max = - MAX_INT;
    int local_min = + MAX_INT;

    top_queue->zone = zone;

    for(int y = top_queue->top_left_ix_y; y < top_queue->bottom_right_ix_y; ++y)
    {
      for(int x = top_queue->top_left_ix_x; x < top_queue->bottom_right_ix_x; ++x)
      {
#if DEBUG
        printf(" %2d", matrix[y][x]);
#endif
        if( local_max < matrix[y][x])
          local_max = matrix[y][x];
        if( local_min > matrix[y][x])
          local_min = matrix[y][x];

        res[y][x] = zone;
      }
#if DEBUG
      printf("\n");
#endif
    }
#if DEBUG
    printf("------------ tl [%d %d] br[%d %d] max [%d] min [%d] diff [%d]\n",
           top_queue->top_left_ix_x, top_queue->top_left_ix_y,
           top_queue->bottom_right_ix_x, top_queue->bottom_right_ix_y,
           local_max,
           local_min,
           local_max - local_min);
#endif
    if( abs(local_max - local_min) > epsilon )
    {
      split(top_queue);
      enqueue(&nodes, &top_queue->children[0]);
      enqueue(&nodes, &top_queue->children[1]);
      enqueue(&nodes, &top_queue->children[2]);
      enqueue(&nodes, &top_queue->children[3]);

      qt->number_of_zones += 3;
      qt->number_of_splits++;
      zone--;
    }

    top_queue->node_max = local_max;
    top_queue->node_min = local_min;

    zone++;

  }

  printf("Matrix after split\n");
  for(int i = 0; i < m_size; ++i)
  {
    for (int j = 0; j < m_size; ++j)
      printf("%4d ", res[i][j]);
    printf("\n");
  }

  printf("-----------------------------------------------\n");
  clearQueue(&nodes);

  return qt;
}

float distance(float x1, float y1, float x2, float y2)
{
  return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

float* calculate_distances(float radius, int number_of_distances)
{

  float* distances = malloc(sizeof(float) * number_of_distances);

  

  distances[0] = radius * 2;
  for(int i = 1; i <= number_of_distances; ++i)
    distances[i] = sqrt( pow(radius + radius / pow(2, i), 2) + pow(radius - radius / pow(2, i), 2) );

  return distances;
}


bool segments_are_neighbors(segment segA, segment segB, float* distances, int number_of_distances)
{
  float sA_mid_point_x = (segA.node->top_left_ix_x + segA.node->bottom_right_ix_x) / 2.0;
  float sA_mid_point_y = (segA.node->top_left_ix_y + segA.node->bottom_right_ix_y) / 2.0;
  float sB_mid_point_x = (segB.node->top_left_ix_x + segB.node->bottom_right_ix_x) / 2.0;
  float sB_mid_point_y = (segB.node->top_left_ix_y + segB.node->bottom_right_ix_y) / 2.0;

  float segments_distance = distance(sB_mid_point_y, sB_mid_point_x, sA_mid_point_y, sA_mid_point_x);

  float dt = distances[number_of_distances -
                       (int)(log(segB.node->bottom_right_ix_x - segB.node->top_left_ix_x) / log(2)) - 1];

  return segments_distance <= dt;
}

int cmp(const void* a, const void* b)
{
  node na = *((segment*)a)->node;
  node nb = *((segment*)b)->node;

  return na.zone > nb.zone;
}

quad_tree* merge_tree(quad_tree* qt,
                      int size,
                      int epsilon)
{
  segment* segments = (segment*) malloc(sizeof(struct segment) * qt->number_of_zones);

  find_terminals(qt, segments);

  qsort(segments, qt->number_of_zones, sizeof(segment), cmp);

  graph* graph = graph_init(qt->number_of_zones);

  for(int i = 0; i < qt->number_of_zones; ++i)
  {
    float radius = segments[i].node->bottom_right_ix_x - segments[i].node->top_left_ix_x;

    int number_of_distances = 1 + (int) (log(radius) / log(2));
    float* distances = calculate_distances(radius / 2, number_of_distances);

    for(int j = i + 1; j < qt->number_of_zones; ++j)
      if(segments_are_neighbors(segments[i], segments[j], distances, number_of_distances) )
        graph_new_edge(graph, i, j);


  }

  partition* partitions = make_sets(qt->number_of_zones);

  bool flag = true;
  while(flag)
  {
    flag = false;
    for(int i = 0; i < qt->number_of_zones; ++i)
    {
      segment* si = &segments[i];
      for(int j = 0; j < (int) graph->nodes[i].successors->size; ++j)
      {
        segment* sj = &segments[graph->nodes[i].successors->array[j]];

        int mmax = MAX(si->node->node_max, sj->node->node_max);
        int mmin = MIN(si->node->node_min, sj->node->node_min);

        if( (abs(mmax - mmin) <= epsilon) && si->zone != sj->zone )
        {
          sj->node->node_max = mmax;
          sj->node->node_min = mmin;
          si->node->node_max = mmax;
          si->node->node_min = mmin;
          sj->zone = MIN(si->zone, sj->zone);
          si->zone = MIN(si->zone, sj->zone);

          flag = true;
        }
      }
    }
  }

  int res[size][size];

  for(int i = 0; i < qt->number_of_zones; ++i)
  {
    segment* s = &segments[i];

    for(int i = s->node->top_left_ix_y; i < s->node->bottom_right_ix_y; ++i)
      for(int j = s->node->top_left_ix_x; j < s->node->bottom_right_ix_x; ++j)
        res[i][j] = s->zone;
  }

  printf("Matrix after merge\n");
  for(int i = 0; i < size; ++i)
  {

    for(int j = 0; j < size; ++j)
    {
      printf("%4d ", res[i][j]);
    }

    printf("\n");
  }

  destroy_graph(graph, qt->number_of_zones);

  free(segments);
  free(partitions);

  return qt;
}

int find_terminal_nodes(node* node, int index, segment* segments)
{
  if(!node)
    return index;

  index = find_terminal_nodes(node->children[0], index, segments);
  index = find_terminal_nodes(node->children[1], index, segments);
  index = find_terminal_nodes(node->children[2], index, segments);
  index = find_terminal_nodes(node->children[3], index, segments);

  if(node->is_terminal)
  {
    segments[index].node = node;
    segments[index].node = node;

    segments[index].seg_max = node->node_max;
    segments[index].seg_min = node->node_min;

    segments[index].has_been_merged = false;
    segments[index].zone = node->zone;

    index++;
  }

  return index;
}

void find_terminals(quad_tree* qt, segment* segments)
{
  find_terminal_nodes(qt->root, 0, segments);
}

quad_tree* new_quad_tree(int brix_x, int brix_y)
{
  quad_tree* qt = (quad_tree *) malloc(sizeof(quad_tree));

  qt->root = new_node(qt->root, ROOT, 0, 0,0, brix_x, brix_y);
  qt->number_of_zones = 1;
  qt->number_of_splits = 0;

  return qt;
}

node* new_node(node* parent,
               DIRECTION type,
               int depth,
               int tlix_x,
               int tlix_y,
               int brix_x,
               int brix_y)
{


  node* new_node = (node* )malloc(sizeof(node));

  new_node->type = type;
  new_node->parent = parent;
  new_node->is_terminal = true;
  new_node->depth = depth;
  new_node->top_left_ix_x = tlix_x;
  new_node->top_left_ix_y = tlix_y;
  new_node->bottom_right_ix_x = brix_x;
  new_node->bottom_right_ix_y = brix_y;

  new_node->is_atomic = (tlix_x == brix_x - 1) && (tlix_y == brix_y - 1);


  return new_node;
}

void destroy_node(node* node)
{
  if(!node)
    return;

  destroy_node(node->children[0]);
  destroy_node(node->children[1]);
  destroy_node(node->children[2]);
  destroy_node(node->children[3]);

  free(node);
}

void destroy_tree(quad_tree* qt)
{
  destroy_node(qt->root);

  free(qt);
}
