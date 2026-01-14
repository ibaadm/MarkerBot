#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include "game_state.h"

typedef struct QueueNode {
    Coord data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

void init_queue(Queue *q);
void enqueue(Queue *q, Coord pos);
Coord dequeue(Queue *q);
bool is_empty(const Queue *q);
void free_queue(Queue *q);

#endif