# QuadTree

A quadtree is a special type of tree in which each node has 4 children. In image processing, quadtress can be used
to split an image into smaller parts, stopping when the condition of the absolute difference between the maximum and minimum value
being smaller than some `threshold` is met.

$$ \| max(f_P) - min(f_P) \| \leq \mathcal{E} \forall P(x,y) \in R $$

QuadTrees usually support 2 main types of operations, `split` and `merge`. 

## Split

A "split" refers to the process of dividing a quadrant into four smaller quadrants. This is done when the pixels within a quadrant 
have different colors and cannot be represented by a single pixel.

When a split occurs, a new set of four child nodes is created for the quadrant, and the pixels within the quadrant are divided among the child nodes. 
Each child node represents one of the four quadrants that the original quadrant is divided into.

The process of splitting can be repeated recursively as needed, with each child node being split into four smaller quadrants if necessary. This 
continues until the desired level of detail is achieved, or until all of the pixels within a quadrant have the same color and can be represented by a 
single pixel.

## Merge 

A "merge" refers to the process of combining four child nodes into a single parent node. This is done when all of the pixels within the four child 
nodes have the same color, and the child nodes can be represented by a single pixel.

When merging we must have a way of creating adjancencies between two candidate quadrants to be merged. In this implementation, the quadrants were
sorted by their size ( number of cells ), and we can try to find the smaller or equal quadrants in size by comparing the distance from the centers 
with a pre-computed value:

$$ dist = \sqrt{ \left(radius + \frac{radius}{2^{i}} \right)^{2} + \left(radius - \frac{radius}{2^{i}}\right)^{2}} $$

In this case, `radius` is the size of a target quadrant, and the powers of 2 to `i` are the smaller squared quadrants.

And this allows us to see if two quadrants are neighbors:

```c++

bool segments_are_neighbors(segment segA, segment segB, float* distances, int number_of_distances)
{
  
  // midpoints
  float sA_mid_point_x = (segA.node->top_left_ix_x + segA.node->bottom_right_ix_x) / 2.0;
  float sA_mid_point_y = (segA.node->top_left_ix_y + segA.node->bottom_right_ix_y) / 2.0;
  float sB_mid_point_x = (segB.node->top_left_ix_x + segB.node->bottom_right_ix_x) / 2.0;
  float sB_mid_point_y = (segB.node->top_left_ix_y + segB.node->bottom_right_ix_y) / 2.0;

  float segments_distance = distance(sB_mid_point_y, sB_mid_point_x, sA_mid_point_y, sA_mid_point_x);

  float dt = distances[number_of_distances -
                       (int)(log(segB.node->bottom_right_ix_x - segB.node->top_left_ix_x) / log(2)) - 1];

  return segments_distance <= dt;
}
```

Having a way to know if two quadrants are neighbors ( or adjacent ) we can think of making up a graph.

``` c++

// make up a graph with number of quadrants after splitting as nodes.
graph* graph = graph_init(qt->number_of_zones);

for(int i = 0; i < qt->number_of_zones; ++i)
{
  // a quadrant's radius 
  float radius = segments[i].node->bottom_right_ix_x - segments[i].node->top_left_ix_x;

  // number of smaller quadrants + 1 ( counting for quadrants with the same size )
  int number_of_distances = 1 + (int) (log(radius) / log(2));
  
  // apply the dist computation
  float* distances = calculate_distances(radius / 2, number_of_distances);

  // look forward for neighbors
  for(int j = i + 1; j < qt->number_of_zones; ++j)
    if(segments_are_neighbors(segments[i], segments[j], distances, number_of_distances) )
      // new undirected edge (i, j) 
      graph_new_edge(graph, i, j);
}
```

Now through exaustion we can inquire if an adjacency can be merged:

``` c++
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

        // a merge occurred
        flag = true;
      }
    }
  }
}
```

## Results

With this approach, relatively slow, we can achieve a QuadTree with Split+Merge, with the following example and with a threshold of `2`:

```

Matrix 
 11  12  10  11  12  10  12  11
 11  11  12  10  10  11   6   5
 10  12  12  11  12   7   6   6
 12  12  11  11   6   7   7   7
 11  10   5   6   6   6   6   7
 12  12   4   6   5   5   5   5
 11   5   6   6   7   7   5   7
  6   5   6   6   7   6   5   5
-----------------------------------------------
Matrix after split
   1    1    1    1    3    3    8    9
   1    1    1    1    3    3   11   10
   1    1    1    1   12   13    4    4
   1    1    1    1   15   14    4    4
   5    5    6    6    2    2    2    2
   5    5    6    6    2    2    2    2
  16   17    7    7    2    2    2    2
  19   18    7    7    2    2    2    2
-----------------------------------------------
Matrix after merge
   1    1    1    1    1    1    1    1
   1    1    1    1    1    1    2    2
   1    1    1    1    1    2    2    2
   1    1    1    1    2    2    2    2
   1    1    6    6    2    2    2    2
   1    1    6    6    2    2    2    2
   1    2    2    2    2    2    2    2
   2    2    2    2    2    2    2    2
```
