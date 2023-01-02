//
// Created by sk on 11/11/22.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H

#include <stdbool.h>

typedef enum DIRECTION
{
  ROOT,
  NW,
  NE,
  SW,
  SE
} DIRECTION;

typedef struct node
{
    DIRECTION type;

    struct node* children[4];

    struct node* parent;
    bool is_terminal;
    bool is_atomic;

    int zone;
    int depth;

    int node_max, node_min;

    int top_left_ix_x;
    int top_left_ix_y;
    int bottom_right_ix_x;
    int bottom_right_ix_y;
}node;

typedef struct quad_tree
{
    node* root;

    int number_of_splits;
    int number_of_zones;
    int max_depth;
}quad_tree;

typedef struct segment
{
    node* node;
    int zone;
    int seg_max, seg_min;
    bool has_been_merged;
}segment;

typedef struct coord
{
   int x;
   int y;
} coords;

node* split(node* Node);

quad_tree* split_tree(quad_tree* tree,
                      int m_size,
                      int matrix[m_size][m_size],
                      int epsilon);

quad_tree* merge_tree(quad_tree* tree,
                      int size,
                      int epsilon);

void find_terminals(quad_tree* tree, segment* segments);

void print_tree(quad_tree* tree);
void print_node(node* node);

quad_tree* new_quad_tree(int brix_x, int brix_y);

node* new_node(node* parent,
               DIRECTION type,
               int depth,
               int tlix_x,
               int tlix_y,
               int brix_x,
               int brix_y);

void destroy_tree(quad_tree* tree);

#endif //QUADTREE_QUADTREE_H
