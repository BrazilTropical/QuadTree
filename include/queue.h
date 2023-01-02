//
// Created by sk on 11/12/22.
//

#ifndef QUADTREE_QUEUE_H
#define QUADTREE_QUEUE_H

typedef struct Node
{
    void *data;
    struct Node *next;
}q_node;

typedef struct QueueList
{
    int sizeOfQueue;
    size_t memSize;
    q_node *head;
    q_node *tail;
}Queue;

void queueInit(Queue *q, size_t memSize);
int enqueue(Queue *, const void *);
void dequeue(Queue *, void *);
void queuePeek(Queue *, void *);
void clearQueue(Queue *);
int getQueueSize(Queue *);

#endif //QUADTREE_QUEUE_H
