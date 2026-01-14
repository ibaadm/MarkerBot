#include "queue.h"
#include <stdio.h>

static void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Fatal Error: Queue node memory allocation failed.\n");
        exit(EXIT_FAILURE); 
    }
    return ptr;
}

void init_queue(Queue *q) {
    q->front = NULL;
    q->rear = NULL;
}

void enqueue(Queue *q, Coord pos) {
    QueueNode *new_node = (QueueNode *)xmalloc(sizeof(QueueNode));
    
    new_node->data = pos;
    new_node->next = NULL;
    
    if (is_empty(q)) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

Coord dequeue(Queue *q) {
    if (is_empty(q)) {
        fprintf(stderr, "Fatal Error: Attempted to dequeue from an empty queue.\n");
        exit(EXIT_FAILURE); 
    }
    
    QueueNode *temp_node = q->front;
    Coord data_to_return = temp_node->data;
    
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp_node);
    
    return data_to_return;
}

bool is_empty(const Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    QueueNode *current = q->front;
    QueueNode *next_node;
    
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    
    q->front = NULL;
    q->rear = NULL;
}