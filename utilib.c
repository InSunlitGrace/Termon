#include "h/utilib.h"
#include "h/vcanvas.h"
#include "h/termon.h"
#include "h/player.h"
#include "h/saves.h"

#include "h/constants.h"

#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

/*
void delay(float seconds) {
    clock_t start_time = clock();
    while (clock() < start_time + seconds * CLOCKS_PER_SEC);
    return;
}
*/

void gexitd(int code){
    endwin();
    if(code==1){
        printDex(masterTerdex);
        printPlayerData();
        printVmap(Vmap);
    }
    if(Vmap) freeVmap(Vmap);
    if(thePlayer) freePlayer();
    if(masterTerdex) freeDex(masterTerdex);
    exit(0);
    return;
}
void texit(){
    endwin();
    exit(0);
}
char **ascii(int id){
    char address[30];
    sprintf(address, "./data/%d.tico", id);

    FILE *fpt = fopen(address, "r");
    if (!fpt)
        return NULL;
    char **icon = malloc(sizeof(char *) * 4);
    char line[16];
    int lino = 0;

    while (lino < 4 && fgets(line, sizeof(line), fpt)){
        line[strcspn(line, "\n")] = '\0';
        icon[lino] = malloc(9);
        strncpy(icon[lino], line, 8);
        icon[lino][8] = '\0';
        lino++;
    }
    fclose(fpt);
    return icon;
}
void cleanString(char * arr, int length){
    for(int i=0;i<length;i++) arr[i]='\0';
    return;
}
void rmnlString(char * arr){
    *(arr + strcspn(arr,"\n"))='\0';
}
void crypt(sline *head) {
    const char key[] = "InSunlitGrace!";  // Change this
    int key_len = strlen(key);
    int key_idx = 0;

    sline *cur = head;
    while (cur) {
        for (int i = 0; i < 513 && cur->contents[i] != '\0'; i++) {
            cur->contents[i] ^= key[key_idx % key_len];
            key_idx++;
        }
        cur = cur->next;
    }
}

void freeSprite(char ** sprite){
    for(int i=0;i<4;i++){
        free(*(sprite+i));
    }
    free(sprite);
}