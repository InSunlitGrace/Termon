#pragma once
#include <ncurses.h>
#include "player.h"
#include "vcanvas.h"
#include "termon.h"

int checkBound(int x, int y, int r, int c);
WINDOW * initOverWorld();
void handleMove(WINDOW * win, int dir, dex * theTerdex, WINDOW ** oOverWorld);
void handleSpawn(WINDOW * win, dex * theTerdex);
void handleDrop();