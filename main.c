#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "h/overworld.h"
#include "h/saves.h"
#include "h/termon.h"
#include "h/ui.h"
#include "h/player.h"
#include "h/vcanvas.h"
#include "h/utilib.h"
#include "h/pvp.h"
#include "h/constants.h"

int main(int argc, char * argv[]){   
    srand(time(NULL));
    int isPVP=0;
    initscr();
    
    if((argc==2 && (strcmp(argv[1],"colour")==0)) ||( (argc==3 && (strcmp(argv[2],"colour")==0)) && (argc==3 && (strcmp(argv[1],"pvp")==0)) )){
        if (!has_colors()) {
            endwin();
            printf("Terminal does not support colors\n");
            exit(0);
        }
        if(argc==3 && (strcmp(argv[1],"pvp")==0)) isPVP=1;
        start_color();
        use_default_colors();
        init_pair(BOXCOLOUR, COLOR_RED, -1);
        init_pair(HEADCOLOUR, COLOR_GREEN, -1);
        init_pair(3,COLOR_YELLOW,-1);
        init_pair(4,COLOR_MAGENTA,-1);
        init_pair(5,COLOR_CYAN,-1);
        init_pair(6,COLOR_BLUE,-1);
        isColour=1;

    }
    else if((argc==2 || argc==3 )&& (strcmp(argv[1],"pvp")==0)){
        isPVP=1;    
    }
    else if(argc==1){
        isColour=0;
        isPVP=0;
    }
    else{
        endwin();
        printf("Incorrect argument! Correct use: \"./game color\" OR \"./game\" or \"./game pvp\" \n");
        exit(0);
    }

    if(!isPVP) printf("\033[8;%d;%dt",MAX_MAP_ROWS+1,MAX_MAP_COLS+MAX_SIDE_COLS+1);
    else printf("\033[8;%d;%dt",20+1, 60+1);
    
    fflush(stdout);
    
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    dex * theTerdex = NULL;
    theCatalogue = NULL;
    thePlayer = NULL;
    Vmap = NULL;

    dispLoad();
    if(isPVP){
        napms(1000);
        pvpMatch();
        exit(0);
    } 
    else{
        napms(500);
    }
    masterTerdex = theTerdex = genDex(DEX_FILE);
    theCatalogue = loadItems();
    napms(2000);
    flushinp();
    dispComand(1,1);
    int ch;
    long long gameMode = showMenu(saveCount());
    thePlayer = handleNewGame(gameMode, theTerdex); 
    Vmap = constructVmap();
    WINDOW * overWorld = initOverWorld();

    if(!overWorld){
        endwin();
        printf("overWorld creation failed\n");
        gexitd(DEBUG);
    }
    wrefresh(overWorld);
    while(1){
        flushinp();
        ch = wgetch(overWorld);

        if(ch==KEY_UP) handleMove(overWorld,DIR_UP, theTerdex, &overWorld);
        else if(ch==KEY_DOWN) handleMove(overWorld,DIR_DOWN, theTerdex, &overWorld);
        else if(ch==KEY_LEFT) handleMove(overWorld,DIR_LEFT, theTerdex, &overWorld);
        else if(ch==KEY_RIGHT) handleMove(overWorld,DIR_RIGHT, theTerdex, &overWorld);
        else if(ch=='T' || ch=='t') showTeam();
        else if(ch=='B' || ch=='b') showBag();
        else if(ch=='H' || ch=='h'){ dispComand(4,8); touchwin(overWorld);}
        else if(ch=='S' || ch=='s') saveFile();
        else if(ch=='Q' || ch=='q') break;

        wrefresh(overWorld);
    }
    
    delwin(overWorld);
    gexitd(DEBUG);

    return 0;
}