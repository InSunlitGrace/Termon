#pragma once

#include "player.h"
#include "vcanvas.h"
#include "termon.h"

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

#define BASEXP 100
#define POWER 50

#define SPLASH "staticscr/splash.txt"
#define COMMANDS "staticscr/commands.txt"

extern int isColour;
extern player * thePlayer;
extern vmap * Vmap;
extern dex * masterTerdex;