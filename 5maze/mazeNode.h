// declares a maze node

#ifndef MAZENODE_H
#define MAZENODE_H

#include <stdbool.h>

typedef struct MazeNode MazeNode;

typedef struct MazePath {
    MazeNode *source;
    MazeNode *target;

    int weight;
} MazePath;

typedef struct MazeNode {
    int index;
    bool hasSwitch;

    MazePath **leavingPaths;
    int leavingPathCount;
    int leavingPathLimit;

    MazePath **arrivingPaths;
    int arrivingPathCount;
    int arrivingPathLimit;
} MazeNode;

MazeNode *newMazeNode(void);
MazePath *newMazePath(MazeNode *sourceNode, MazeNode *targetNode, int weight);
void freeMazeNode(MazeNode *mazeNode);
void freeMazePath(MazePath *mazePath);

#endif // MAZENODE_H
