/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#ifndef LIBQUEUE_H
#define LIBQUEUE_H

#include "EditOperation.h"

typedef struct Queue {
  EditOperation *array;
  int back;
  int front;
  int size;
} Queue;

Queue newQueue(int s);
int isEmptyQueue(Queue q);
void doubleQueueSize(Queue *qp);
void enqueue(EditOperation item, Queue *qp);
EditOperation dequeue(Queue *qp);
void freeQueue(Queue q);

#endif
