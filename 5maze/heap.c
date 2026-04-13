#include <stdlib.h>
#include "memory.h"
#include "heap.h"

// Only a single heap is needed for this program, so this structure only 
// manipulates this heap rather than allowing multiple heaps to be created and manipulated
HeapNode **heap;
int heapSize = 0;
int heapLimit = 8;

// This is seperate because "heap" changes in runtime, and references to all the heapNodes will become difficult to track otherwise
// So, all created HeapNodes are immediately stored in here so that they can easily be freed
HeapNode **historicalHeap;
int historicalHeapSize = 0;
int historicalHeapLimit = 8;

// Cannot be done above as it cannot be statically allocated
void initHeap(void) {
    heap = safeMalloc(sizeof(HeapNode *) * heapLimit);
    historicalHeap = safeMalloc(sizeof(HeapNode *) * heapLimit);
}

// Frees the heap
void freeHeap(void) {
    for (int i = 0; i < historicalHeapSize; i ++) {
        free(historicalHeap[i]);
    }
    free(historicalHeap);
    free(heap);
}

// Swaps two items in the heap
void swap(int i, int j) {
    HeapNode *orig = heap[i];
    heap[i] = heap[j];
    heap[j] = orig;
}

// Double the available size of the heap
void doubleHeapSize(void) {
    heapLimit *= 2;
    heap = safeRealloc(heap, sizeof(HeapNode *) * heapLimit);
}

// Double available size of the historical heap
void doubleHistoricalHeapSize(void) {
    historicalHeapLimit *= 2;
    historicalHeap = safeRealloc(historicalHeap, sizeof(HeapNode *) * historicalHeapLimit);
}

// Push a new item to the heap with the MazeNode index, the current state in the algorithm, and the current distance
void push(int nodeIndex, int state, int distance) {
    HeapNode *heapNode = safeMalloc(sizeof(HeapNode));
    heapNode->nodeIndex = nodeIndex;
    heapNode->state = state;
    heapNode->distance = distance;

    // Note that indexing starts at 1 so that index / 2 works
    if (heapSize >= heapLimit) {
        doubleHeapSize();
    }
    heapSize += 1;
    heap[heapSize] = heapNode;

    // Here only a record of heapNodes is needed, so indexing can start at 0
    if (historicalHeapSize >= historicalHeapLimit) {
        doubleHistoricalHeapSize();
    }
    historicalHeap[historicalHeapSize] = heapNode;
    historicalHeapSize += 1;

    // Maintain heap structure
    int i = heapSize;
    while (i > 1 && heap[i]->distance < heap[i/2]->distance) {
        swap(i, i/2);
        i = i / 2;
    }
}

// Removes the item from the top of the heap and returns it
HeapNode *pop(void) {
    HeapNode *topNode = heap[1];

    // Moves the last item to the top
    heap[1] = heap[heapSize];
    heapSize -= 1;

    // Maintain heap structure again
    int i = 1;
    while (2 > 1) { // todo: check if this will always be true
        int smallest = i;
        int left = 2 * i;
        int right = 2 * i + 1;

        // Checks if left/right child exists, and if its smaller than the parent (or the current smallest item)
        if (left <= heapSize && heap[left]->distance < heap[smallest]->distance) {
            smallest = left;
        }
        if (right <= heapSize && heap[right]->distance < heap[smallest]->distance) {
            smallest = right;
        }

        // Heap structure is ready
        if (smallest == i) {
            break;
        }

        // Swap this parent with the smallest child and iterate
        swap(i, smallest);
        i = smallest;
    }

    return topNode;
}

// Returns true if theres nothing left in the heap
bool isEmpty(void) {
    return heapSize == 0;
}
