#ifndef PARTITIONS_H_
#define PARTITIONS_H_

#include <stdbool.h>

typedef struct partition
{
  int parent;
  int rank;
} partition;

partition* make_sets(int n);

int ds_find(partition* set, int x);

bool ds_union(partition* set, int x, int y);

#endif // PARTITIONS_H_
