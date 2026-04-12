#pragma once
#include <stdlib.h>

typedef struct map{
    int rows;
    int cols;
    int spawnCount;
    int * spawns;
    int baseSpawnLevel;
    char ** map;
    int nx,ny,px,py;
} vmap;

vmap * constructVmap();
char getVmapChar(vmap * Vmap, int x, int y);
void printVmap(vmap * Vmap);
void freeVmap(vmap * Vmap);
