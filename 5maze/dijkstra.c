#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "dijkstra.h"
#include "memory.h"
#include "heap.h"
#include "mazeNode.h"

// Function to double the limits of pathNode and pathState arrays
void doublePathArrayLimits(int **pathNode, int **pathState, int *pathLimit) {
    *pathLimit *= 2;
    *pathNode = safeRealloc(*pathNode, sizeof(int) * (*pathLimit));
    *pathState = safeRealloc(*pathState, sizeof(int) * (*pathLimit));
}

// Solve the specified maze and returns a DijkstraResult struct with relevant arrays created in the process
DijkstraResult dijkstra(MazeNode **nodes, int nodeCount) {
    size_t blockSize = sizeof(int[nodeCount + 1][2]);

    // Allocate [nodeCount]x[2] arrays for the state
    int (*distances)[2] = safeMalloc(blockSize);
    int (*visited)[2] = safeMalloc(blockSize);

    int (*parentNode)[2] = safeMalloc(blockSize);
    int (*parentState)[2] = safeMalloc(blockSize);
    int (*usedSwitch)[2] = safeMalloc(blockSize);

    // Initialize the state
    for (int i = 1; i <= nodeCount; i ++) {
        distances[i][0] = INT_MAX;
        visited[i][0] = 0;

        // Reverse states
        distances[i][1] = INT_MAX;
        visited[i][1] = 0;
    }

    // First node has 0 distance
    distances[1][0] = 0;
    push(1, 0, 0);

    // Go through the entire heap of nodes to be checked
    while (!isEmpty()) {
        // Gets the HeapNode at the top of the heap
        HeapNode *current = pop();

        // index of the node being looked at
        int uIndex = current->nodeIndex; 
        // state of the node, 0: global switch is off, 1: global switch is on
        int uState = current->state;
        // the actual node pointer
        MazeNode *uNode = nodes[uIndex];

        // Skip this if it was already visited, otherwise record the visit in the state
        if (visited[uIndex][uState] == 1) {
            continue;
        }
        visited[uIndex][uState] = 1;

        int pathCount;
        MazePath **pathArray;
        if (uState == 0) {
            // if state is 0, then global switch is off, meaning standard path directions are used 
            pathCount = uNode->leavingPathCount;
            pathArray = uNode->leavingPaths;
        } else {
            // if state is 1, then global switch is ON meaning reversed path directions are used
            pathCount = uNode->arrivingPathCount;
            pathArray = uNode->arrivingPaths;
        }

        // iterate through all paths leaving (or entering, depending on state) the node
        for (int i = 0; i < pathCount; i ++) {
            MazePath *path = pathArray[i];
            // the relevant field will be different depending on the state
            MazeNode *vNode = uState == 0 ? path->target : path->source;
            int weight = path->weight;

            // grow the distance
            int newDist = distances[uIndex][uState] + weight;

            // if this is a better path, then record it in the state and push it to the heap to be further checked
            if (newDist < distances[vNode->index][uState]) {
                distances[vNode->index][uState] = newDist;

                parentNode[vNode->index][uState] = uIndex;
                parentState[vNode->index][uState] = uState;

                usedSwitch[vNode->index][uState] = 0;

                push(vNode->index, uState, newDist);
            }
        }

        // Check the potential cost of flipping the switch and reversing the global path directions
        if (uNode->hasSwitch) {
            int flippedState = 1 - uState;
            int uDistance = distances[uIndex][uState];

            // Same as earlier, better path was found so record it
            if (uDistance < distances[uIndex][flippedState]) {
                distances[uIndex][flippedState] = distances[uIndex][uState];

                parentNode[uIndex][flippedState] = uIndex;
                parentState[uIndex][flippedState] = uState;

                usedSwitch[uIndex][flippedState] = 1;

                push(uIndex, flippedState, distances[uIndex][flippedState]);
            }
        }
    }

    DijkstraResult result = {distances, visited, parentNode, parentState, usedSwitch};
    return result;
}

// Prints the result of running the algorithm in the specified format
void printDijkstraResult(DijkstraResult result, int amountOfNodes) {
    int finalState = 1;
    if (result.distances[amountOfNodes][0] <= result.distances[amountOfNodes][1]) {
        finalState = 0;
    }

    if (result.distances[amountOfNodes][finalState] == INT_MAX) {
        // Max distance implies the final node was never visited in the algorithm
        printf("IMPOSSIBLE\n");
    } else {
        printf("%d\n", result.distances[amountOfNodes][finalState]);

        // Dynamically resized arrays to record the path
        int *pathNode = safeMalloc(sizeof(int) * 8);
        int *pathState = safeMalloc(sizeof(int) * 8);
        int pathCount = 0;
        int pathLimit = 8;

        // Start from the final maze node
        int nodeIndex = amountOfNodes;
        int nodeState = finalState;

        // At the beginning of the maze, index will be 1 (first node) and state 
        // has to be 0, so anything else is an in-progress path
        while (nodeIndex != 1 || nodeState != 0) {
            if (pathCount >= pathLimit) {
                doublePathArrayLimits(&pathNode, &pathState, &pathLimit);
            }

            // Record this step in the path 
            pathNode[pathCount] = nodeIndex;
            pathState[pathCount] = nodeState;
            pathCount += 1;

            // Get the previously visited node
            int parentNodeIndex = result.parentNode[nodeIndex][nodeState];
            int parentNodeState = result.parentState[nodeIndex][nodeState];

            nodeIndex = parentNodeIndex;
            nodeState = parentNodeState;
        }

        // Add the very first node to the array
        if (pathCount >= pathLimit) {
            doublePathArrayLimits(&pathNode, &pathState, &pathLimit);
        }
        pathNode[pathCount] = 1;
        pathState[pathCount] = 0;
        pathCount += 1;

        int recentlyPrinted = -1;

        // Print the recorded path in reverse (because it was traversed from end to beginning)
        for (int i = pathCount - 1; i >= 0; i --) {
            int uNodeIndex = pathNode[i];
            // int uNodeState = pathState[i];

            // Dont double print any nodes
            if (uNodeIndex == recentlyPrinted) {
                continue;
            }

            printf("%d", uNodeIndex);

            // Print R if the switch was used
            if (i > 0 && pathNode[i] == pathNode[i - 1] && result.usedSwitch[pathNode[i - 1]][pathState[i - 1]]) {
                printf(" R");
            }
            
            printf("\n");
            recentlyPrinted = uNodeIndex;
        }

        free(pathNode);
        free(pathState);
    }
}

// Fre the result struct created by the algorithm
void freeDijkstraResult(DijkstraResult result) {
    free(result.distances);
    free(result.visited);
    free(result.parentNode);
    free(result.parentState);
    free(result.usedSwitch);
}
