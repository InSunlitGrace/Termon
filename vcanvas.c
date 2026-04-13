#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#include "h/vcanvas.h"
#include "h/constants.h"

//CONSTRUCTS THE CURRENT OVERWORLD MAP
vmap * constructVmap(){
    char fileName[100];
    char dataFileName[100];
    for(int i=0; i<100; i++) dataFileName[i] = fileName[i]='\0';

    sprintf(dataFileName,"./maps/%d.mpd", thePlayer->map);
    sprintf(fileName,"./maps/%d.mp", thePlayer->map);

    FILE * mpd = fopen(dataFileName, "r");
    if(!mpd){
        
        endwin();
        printf("ERROR! MAP DATA %d NOT FOUND\n", thePlayer->map);
        printf("%s\n",dataFileName);
        exit(1);
    }

    vmap * Vmap = (vmap *)malloc(sizeof(vmap));

    fscanf(mpd,"%d %d", &(Vmap->rows), &(Vmap->cols));
    fscanf(mpd,"%d %d", &(Vmap->nx), &(Vmap->ny));
    fscanf(mpd,"%d %d", &(Vmap->px), &(Vmap->py));
    fscanf(mpd,"%d", &(Vmap->spawnCount));
    fscanf(mpd,"%d", &(Vmap->baseSpawnLevel));
    int * spawns = (int *)malloc(sizeof(int)*(Vmap->spawnCount));
    for(int i=0;i<(Vmap->spawnCount);i++){
        fscanf(mpd,"%d,",(spawns + i));
    }
    Vmap->spawns=spawns;
    fclose(mpd);
    int rows,cols;
    rows=Vmap->rows;
    cols=Vmap->cols;

    Vmap->map = (char **)malloc(sizeof(char *)*(rows));
    for(int i=0; i<(rows); i++){
        *((Vmap->map)+i)=(char *)malloc(sizeof(char)*(cols+1));
    }


    FILE * mp = fopen(fileName,"r");
    if(!mp){
        endwin();
        printf("ERROR! MAP %d NOT FOUND\n", thePlayer->map);
        exit(1);
    }
    for(int i=0; i<rows; i++){
        fgets(*((Vmap->map)+i), cols+2, mp);
        (*(Vmap->map + i))[cols]='\0';
    }
    return Vmap;
}

//RETURNS THE TILE AT SPECIFIC X,Y ON A MAP
char getVmapChar(vmap * Vmap, int x, int y){
    int rows = Vmap->rows;
    int cols = Vmap->cols;
    if(x>=rows || x<0 || y>=cols || y<0) return '\0';
    return *(*(Vmap->map+x)+y);
}

//UTILS
void freeVmap(vmap * Vmap){
    char ** map = Vmap->map;
    int rows= Vmap->rows;
    for(int i=0; i<rows; i++){
        free(*(map+i));
    }
    free(Vmap);
}

//DEBUG
void printVmap(vmap * Vmap){
    int rows = Vmap->rows;
    int cols = Vmap->cols;
    for(int i=0; i<rows; i++){
        for(int j=0;j<=cols;j++){
            printf("%c",(*(*((Vmap->map) +i)+j)));
        }
        printf("\n");
    }
    printf("Rows: %d, Cols: %d, SpawnCount: %d\n", rows,cols,Vmap->spawnCount);
    printf("Spawns: ");
    for(int i=0;i<(Vmap->spawnCount);i++){
        printf("%d ", *((Vmap->spawns) +i));
    }
    printf("\n");
}