#pragma once
#include <ncurses.h>

#include "termon.h"
#include "vcanvas.h"

#define MAX_ITEM_NAME_LEN 9
#define MAX_ITEM_DESC_LEN 29

typedef struct player_bag_item{
    int index;
    char itemName[MAX_ITEM_NAME_LEN +1];
    int itemQuant;
    char description[MAX_ITEM_DESC_LEN +1];
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
    long long savefile;
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
void addItem(int id, int quant);

void reindexTeam();

void printPlayerData();
void printTeamData(team * theTeam);
void printTeamData2(team * theTeam);
void printBagData(bag * theBag);

void freeBag();
void freeTeam();
void freePlayer();