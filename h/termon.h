#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct individual{
    int hpI;
    int atkI;
    int defI;
    int spdI;

    int id;
    int hp;
    int atk;
    int def;
    int spd;

    long unsigned int xp;
    int health;
    char name[13];

    int lvl;
} termon;

typedef struct list{
    int id;
    char name[13];
    int b_hp;
    int b_atk;
    int b_def;
    int b_speed;
    struct list * next;
} dex;

dex * genDex(char arr[]);
dex * nFromDex(dex * start, int id);
void printDex(dex * start);

int genI();
int genStat(int base, int lvl, int IV);
long unsigned int genStartXP(int lvl);

termon * genNewTermon(dex * Terdex, int id, int lvl);
void freeDex(dex * theTerdex);