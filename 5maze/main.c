#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "mazeNode.h"
#include "memory.h"
#include "heap.h"
#include "dijkstra.h"

int main(void){
    // ------------ Read input for node count and create nodes ------------ //
    int amountOfNodes = 0;
    int amountOfPaths = 0;
    scanf("%d %d", &amountOfNodes, &amountOfPaths);

    // Note that indexing starts at 1
    MazeNode **nodes = safeMalloc(sizeof(MazeNode *) * (amountOfNodes + 1));
    for (int i = 1; i <= amountOfNodes; i++){ 
        MazeNode *node = newMazeNode();
        node->index = i;
        nodes[i] = node;
    }

    // ------------ Read which nodes have a switch ------------ //
    int hasSwitchInput = 0;
    scanf("%d", &hasSwitchInput);

    while (hasSwitchInput != -1){
        nodes[hasSwitchInput]->hasSwitch = true;
        scanf("%d", &hasSwitchInput);
    }
    
    // ------------ Read paths between nodes ------------ //
    MazePath **paths = safeMalloc(sizeof(MazePath *) * amountOfPaths);
    for (int i = 0; i < amountOfPaths; i++){
        int idx1 = 0;
        int idx2 = 0;
        int weight = 0;
        scanf("%d %d %d", &idx1, &idx2, &weight);
        
        MazePath * newPath = newMazePath(nodes[idx1], nodes[idx2], weight);
        paths[i] = newPath;
    }

    // ------------ Apply custom Dijkstra algorithm ------------ //
    initHeap();
    DijkstraResult result = dijkstra(nodes, amountOfNodes);
    printDijkstraResult(result, amountOfNodes);

    // ------------ Free allocated memory for objects ------------ //
    freeDijkstraResult(result);

    // First free paths, then nodes because multiple nodes may contain the same path
    for (int i = 0; i < amountOfPaths; i ++) {
        freeMazePath(paths[i]);
    }
    for (int i = 1; i <= amountOfNodes; i ++) {
        freeMazeNode(nodes[i]);
    }
    freeHeap();
    free(nodes);
    free(paths);

    return 0;
}
