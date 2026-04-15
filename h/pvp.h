#include <stdlib.h>

#include "termon.h"
#include "player.h"

typedef struct pvpPlayer
{
    int id;
    team * teamMate;        
} plyr;


void pvpMatch();
