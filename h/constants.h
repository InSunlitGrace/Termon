#pragma once

#include "player.h"
#include "vcanvas.h"
#include "termon.h"
#include <signal.h>

#define DEBUG 0

#define SPLASH "staticscr/splash.txt"
#define COMMANDS "staticscr/commands.txt"
#define SAVECOUNT "saves/saveCount.txt"

#define MAX_MAP_ROWS 35
#define MAX_MAP_COLS 75
#define MAX_SIDE_COLS 40

#define MAX_TICO_ADD_LEN 30
#define MAX_TICO_COLS 9
#define MAX_TICO_ROWS 4

#define MAX_ITEM_NAME_LEN 9
#define MAX_ITEM_DESC_LEN 29

#define BOXCOLOUR 1
#define HEADCOLOUR 2
#define FOOTCOLOUR 6
#define LOGOCOLOUR 3
#define OPTIONCOLOUR 4
#define CONTENTCOLOUR 5
#define WARNING 1
#define SAFE 2

#define SELFCOLOUR 1
#define TREECOLOUR 2
#define BUSHCOLOUR 3
#define GATECOLOUR 4
#define PUDDLECOLOUR 5
#define PLAYERCOLOUR 5

#define STARTER_LEVEL 7
#define MAX_IV 31
#define SPAWN_STEPS 10
#define DROP_STEPS 25
#define BASEXP 100
#define POWER 50

extern int isColour;
extern player * thePlayer;
extern vmap * Vmap;
extern dex * masterTerdex;
extern bag * theCatalogue;

typedef enum {
    DIR_UP    = 1,
    DIR_RIGHT = 2,
    DIR_DOWN  = 3,
    DIR_LEFT  = 4
} Direction;