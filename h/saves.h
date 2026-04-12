#pragma once
#include "player.h"
#include "termon.h"

typedef struct save{
    char contents[513];
    struct save * next;
} sline;

int saveCount();
player * handleNewGame(int mode, dex * terdex,bag * catalogue);
void saveFile();
void rmnlString(char * arr);