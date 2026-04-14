#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "h/overworld.h"
#include "h/saves.h"
#include "h/termon.h"
#include "h/ui.h"
#include "h/player.h"
#include "h/constants.h"

//GENERATES DEX FROM DEX.CSV
dex * genDex(char arr[]){

    FILE * dexfile = fopen(arr, "r");
    if(!dexfile){
        printf("ERROR: Dex File Not Found\n");
        return NULL;
    }

    char line[256];
    for(int i=0; i<256; i++) line[i]='\0';

    dex * firstMon=NULL;
    dex * curMon=firstMon;

    while(fgets(line,255,dexfile)){
        if(!firstMon){
            curMon = firstMon = (dex *)malloc(sizeof(dex));
        }
        else{
            curMon=curMon->next =(dex *)malloc(sizeof(dex));
            
        }
        curMon->id=atoi(strtok(line,","));
        strcpy((curMon->name),strtok(NULL,","));
        curMon->b_hp=atoi(strtok(NULL,","));
        curMon->b_atk=atoi(strtok(NULL,","));
        curMon->b_def=atoi(strtok(NULL,","));
        curMon->b_speed=atoi(strtok(NULL,","));
        curMon->next=NULL;
    }
    return firstMon;

}

//RETURNS A POINTER TO THE NTH ENTRY FROM A DEX
dex * nFromDex(dex * start, int id){
    dex * reqMon=NULL;
    dex * cur = start;
    while(cur){
        if(cur->id == id){
            reqMon=cur;
            break;
        }
        cur=cur->next;
    }
    return reqMon;
}

//UTILITIES
    //GENERATES RANDOM I VALUES FROM 0 TO 31
int genI(){
    return (rand()%(MAX_IV +1));

}
    //GENERATES STATS USING BASE, LEVEL AND I VALUES
int genStat(int base, int lvl, int IV){
    return (int)((((((1.6)*base)+0.5*IV)*lvl)/80)+lvl+10);
}

    //GIVES XP TO NEWLY SPAWNED TERMON AT LEVEL
long unsigned int genStartXP(int lvl){
    return ((((long unsigned int)lvl)*(lvl+1)/2)*BASEXP);
}

//FOR NEW INDIVIDUAL TERMON
termon * genNewTermon(dex * Terdex, int id, int lvl){
    termon * mon = (termon *)malloc(sizeof(termon));
    dex * template = nFromDex(Terdex,id);
    if(!template){
        printf("Pokemon Not Found In Dex To generate\n");
        return NULL;
    }

    mon->id=id;

    mon->hpI=genI();
    mon->atkI=genI();
    mon->defI=genI();
    mon->spdI=genI();

    mon->hp=genStat(template->b_hp, lvl, mon->hpI);
    mon->atk=genStat(template->b_atk, lvl, mon->atkI);
    mon->def=genStat(template->b_def, lvl, mon->defI);
    mon->spd=genStat(template->b_speed, lvl,mon->spdI);

    mon->xp=genStartXP(lvl);
    mon->health=mon->hp;
    strcpy(mon->name, template->name);

    mon->lvl=lvl;
    return mon;

}

//FREE A DEX
void freeDex(dex * theTerdex){
    dex * cur, * next;
    cur = theTerdex;
    while(cur){
        next=cur->next;
        free(cur);
        cur=next;
    }
    return;
}

//DEBUG
void printDex(dex * start){
    dex * cur=start;
    if(cur==NULL){
        printf("Null Dex\n");
        return;
    }
    while(cur){
        printf("%s %d\n", cur->name, cur->b_speed);
        cur=cur->next;
    }
    return;
}
