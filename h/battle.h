#pragma once
#include <ncurses.h>
#include "vcanvas.h"
#include "termon.h"
#include "player.h"

void startBattle(WINDOW * parentWin, dex * theTerdex);
int damCalc(termon * attacker, termon * defender);
int isFainted(termon * mon);
void giveXP(termon * winner, termon * loser);
team * handleTeamFaint(WINDOW * parent, termon * theMon, int * confirm );
int tryCatch(termon * theSpawn);