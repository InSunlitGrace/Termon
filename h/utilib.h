#pragma once
#include <time.h>
#include <ncurses.h>

#include "player.h"
#include "vcanvas.h"
#include "termon.h"
#include "saves.h"


void gexitd(int code);
void gexit_without(int code);
void texit();
char ** ascii(int id);
void cleanString(char * arr, int length);
void crypt(sline * head);
void freeSprite(char ** sprite);
int mdh(int x, int y);