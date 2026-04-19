#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "h/utilib.h"
#include "h/vcanvas.h"
#include "h/termon.h"
#include "h/player.h"
#include "h/saves.h"
#include "h/constants.h"

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
    if(theCatalogue){
        bag * cur, * prev;
        cur=theCatalogue->next;
        prev=theCatalogue;
        while(prev){
            free(prev);
            prev=cur;
            if(cur) cur=cur->next;
        }
    }
    exit(0);
    return;
}

void texit(){
    endwin();
    exit(0);
}

char **ascii(int id){
    char address[MAX_TICO_ADD_LEN];
    sprintf(address, "./data/%d.tico", id);

    FILE *fpt = fopen(address, "r");
    if (!fpt)
        return NULL;
    char **icon = malloc(sizeof(char *) * 4);
    char line[MAX_TICO_COLS +1];
    int lino = 0;

    while (lino < MAX_TICO_ROWS && fgets(line, sizeof(line), fpt)){
        line[strcspn(line, "\n")] = '\0';
        icon[lino] = malloc(MAX_TICO_COLS+1);
        strncpy(icon[lino], line, MAX_TICO_COLS);
        icon[lino][MAX_TICO_COLS] = '\0';
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
    if(!sprite) return;
    for(int i=0;i<MAX_TICO_ROWS;i++){
        free(*(sprite+i));
    }
    free(sprite);
}

int mdh(int x, int y){
    if(x>y) return x;
    return y;
}