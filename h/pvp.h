#include <stdlib.h>

#include "termon.h"
#include "player.h"

typedef struct pvpPlayer
{
    int id;
    team * teamMate;        
} plyr;

void freePlyr(plyr * thePlyr);

int allFaintedPlyr(plyr * aPlyr);

void endGame(int mode);

void pvpMatch();
