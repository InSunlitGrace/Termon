#pragma once
#include "termon.h"
#include "vcanvas.h"
#include <ncurses.h>

typedef struct player_bag_item{
    int index;
    char itemName[10];
    int itemQuant;
    char description[30];
    struct player_bag_item * prev;
    struct player_bag_item * next;
} bag;

typedef struct player_team_mon{
    int teamID;
    termon * mon;
    struct player_team_mon * prev;
    struct player_team_mon * next;

} team;

typedef struct player_obj{
    int map;
    int x;
    int y;
    int savefile;
    team * pTeam;
    bag * pBag;
} player;


player * newPlayer(int starter, dex * terdex);
bag * loadItems();
void addMonTeam(termon * theMon);
void rmTeamMon(team * teamMate, WINDOW * parent, team ** displayedMon);
void healTeamMon(team * teamMate, WINDOW * parent);
void showTeam();
void showBag();
void addItem(bag * catalogue, int id, int quant);

void reindexTeam();

void printPlayerData();
void printTeamData(team * theTeam);
void printTeamData2(team * theTeam);
void printBagData(bag * theBag);

void freeBag();
void freeTeam();
void freePlayer();