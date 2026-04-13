#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>

typedef struct HeapNode {
    int nodeIndex;
    int state;
    int distance;
} HeapNode;

void initHeap(void);
void freeHeap(void);
void push(int nodeIndex, int state, int distance);
HeapNode *pop(void);
bool isEmpty(void);

#endif // HEAP_H
