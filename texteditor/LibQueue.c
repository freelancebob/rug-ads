/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#include "LibQueue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Queue newQueue(int s) {
  Queue q;
  q.array = malloc(s * sizeof(*q.array));
  assert(q.array != NULL);
  q.back = 0;
  q.front = 0;
  q.size = s;
  return q;
}

int isEmptyQueue(Queue q) {
  return (q.back == q.front);
}

void queueEmptyError(void) {
  printf("queue empty\n");
  abort();
}

void doubleQueueSize(Queue *qp) {
  int i;
  int oldSize = qp->size;
  qp->size = 2 * oldSize;
  qp->array = realloc(qp->array, qp->size * sizeof(*qp->array));
  assert(qp->array != NULL);
  for (i = 0; i < qp->back; i++) { /* eliminate the split configuration */
    qp->array[oldSize + i] = qp->array[i];
  }
  qp->back = qp->back + oldSize;  /* update qp->back */
}

void enqueue(EditOperation item, Queue *qp) {
  qp->array[qp->back] = item;
  qp->back = (qp->back + 1) % qp->size;
  if (qp->back == qp->front) {
    doubleQueueSize(qp);
  }
}

EditOperation dequeue(Queue *qp) {
  EditOperation item;
  if (isEmptyQueue(*qp)) {
    queueEmptyError();
  }
  item = qp->array[qp->front];
  qp->front = (qp->front + 1) % qp->size;
  return item;
}

void freeQueue(Queue q) {
  free(q.array);
}
