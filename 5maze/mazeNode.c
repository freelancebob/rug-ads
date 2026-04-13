#include <memory.h>
#include <stdlib.h>
#include <limits.h>

#include "mazeNode.h"
#include "memory.h"

// Creates a new blank maze node
// Most fields are initialized in main.c in runtime
MazeNode *newMazeNode(void) {
    MazeNode *newNode = safeMalloc(sizeof(MazeNode));
    
    newNode->hasSwitch = false;

    // These must be dynamically allocated arrays
    newNode->leavingPaths = safeMalloc(sizeof(MazePath *) * 8);
    newNode->leavingPathCount = 0;
    newNode->leavingPathLimit = 8;

    newNode->arrivingPaths = safeMalloc(sizeof(MazePath *) * 8);
    newNode->arrivingPathCount = 0;
    newNode->arrivingPathLimit = 8;

    return newNode;
}

// Doubles the limit for the leavingPaths array
void doubleLeavingArraySize(MazeNode *node) {
    node->leavingPathLimit *= 2;
    node->leavingPaths = realloc(node->leavingPaths, sizeof(MazePath *) * node->leavingPathLimit);
}
// Doubles the limit for the arrivingPaths array
void doubleArrivingArraySize(MazeNode *node) {
    node->arrivingPathLimit *= 2;
    node->arrivingPaths = realloc(node->arrivingPaths, sizeof(MazePath *) * node->arrivingPathLimit);
}

// Creates a new MazePath, and adds itself to the "...paths" arrays of the source and target nodes
MazePath *newMazePath(MazeNode *sourceNode, MazeNode *targetNode, int weight) {
    MazePath *newPath = safeMalloc(sizeof(MazePath));
    newPath->source = sourceNode;
    newPath->target = targetNode;
    newPath->weight = weight;

    // Dynamically resize arrays
    if (sourceNode->leavingPathCount >= sourceNode->leavingPathLimit) {
        doubleLeavingArraySize(sourceNode);
    }
    sourceNode->leavingPaths[sourceNode->leavingPathCount] = newPath;
    sourceNode->leavingPathCount += 1;

    if (targetNode->arrivingPathCount >= targetNode->arrivingPathLimit) {
        doubleArrivingArraySize(targetNode);
    }
    targetNode->arrivingPaths[targetNode->arrivingPathCount] = newPath;
    targetNode->arrivingPathCount += 1;

    return newPath;
}

// Frees the maze node and its arrays
void freeMazeNode(MazeNode *mazeNode) {
    free(mazeNode->leavingPaths);
    free(mazeNode->arrivingPaths);
    free(mazeNode);
}

// Frees the MazePath
void freeMazePath(MazePath *mazePath) {
    free(mazePath);
}
