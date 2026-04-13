#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "mazeNode.h"

typedef struct DijkstraResult {
    int (*distances)[2];
    int (*visited)[2];

    int (*parentNode)[2];
    int (*parentState)[2];
    int (*usedSwitch)[2];
} DijkstraResult;

DijkstraResult dijkstra(MazeNode **nodes, int nodeCount);
void printDijkstraResult(DijkstraResult result, int amountOfNodes);
void freeDijkstraResult(DijkstraResult result);

#endif // DIJKSTRA_H
