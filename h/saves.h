#pragma once
#include "player.h"
#include "termon.h"

typedef struct save{
    char contents[513];
    struct save * next;
} sline;

long long saveCount();
player * handleNewGame(long long mode, dex * terdex);
void saveFile();
void rmnlString(char * arr);