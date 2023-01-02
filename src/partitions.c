#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "partitions.h"

partition* make_sets(int n)
{
  partition* part = (partition*) malloc(sizeof(partition) * n);

  for(int i = 0; i < n; ++i)
  {
   part[i].parent = 0;
   part[i].rank = i;
  }

  return part;
}

int ds_find(partition* set, int x)
{
  if( set[x].parent != x)
    set[x].parent = ds_find(set, set[x].parent);

  return set[x].parent;
}

bool ds_union(partition* set, int x, int y)
{
  int rx = ds_find(set, x);
  int ry = ds_find(set, y);

  if(set[x].rank < set[y].rank)
  {
    set[x].parent = ry;
  }
  else if(set[x].rank > set[y].rank)
  {
    set[x].parent = rx;
  }
  else
  {
    set[x].parent = rx;
    set[x].rank++;
  }

  return true;
}
