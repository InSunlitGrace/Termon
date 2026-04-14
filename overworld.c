#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

#include "h/overworld.h"
#include "h/vcanvas.h"
#include "h/termon.h"
#include "h/ui.h"
#include "h/player.h"
#include "h/battle.h"
#include "h/utilib.h"
#include "h/constants.h"

//INITIALISES A NEW MAP
WINDOW * initOverWorld(){
    int rows = Vmap->rows, cols = Vmap->cols;
    int starty = 1, startx = 1;

    WINDOW *win = newwin(rows, cols, starty, startx);
    keypad(win, TRUE);
    curs_set(0);

    int clr=0;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(isColour){
                if((*(*((Vmap->map)+i)+j))=='T') clr=TREECOLOUR;
                else if ((*(*((Vmap->map)+i)+j))=='#') clr=BUSHCOLOUR;
                else if ((*(*((Vmap->map)+i)+j))==']' || (*(*((Vmap->map)+i)+j))=='[') clr=GATECOLOUR;
                else if ((*(*((Vmap->map)+i)+j))=='O') clr=PUDDLECOLOUR;
                else if ((*(*((Vmap->map)+i)+j))=='&') clr=PLAYERCOLOUR;
                else clr=0;
                if(clr){
                    wattron(win,COLOR_PAIR(clr));
                }
            }
            mvwaddch(win, i,j,*(*((Vmap->map)+i)+j));
            if(clr && isColour!=0){
                wattroff(win,COLOR_PAIR(clr));
            }
        }
    }
    if(isColour) wattron(win,COLOR_PAIR(1));
    mvwaddch(win,thePlayer->x,thePlayer->y,'@');
    if(isColour) wattroff(win,COLOR_PAIR(1));
    return win;

}

//HANDLES MAP MOVEMENT
void handleMove(WINDOW * win, Direction dir, dex * theTerdex, WINDOW ** oOverWorld){
    int ox=thePlayer->x, oy=thePlayer->y;
    int nx=ox, ny=oy;
    WINDOW * pwin = win;
    switch (dir)
    {
    case DIR_UP:
        nx--;
        break;
    case DIR_RIGHT:
        ny++;
        break;
    case DIR_DOWN:
        nx++;
        break;
    case DIR_LEFT:
        ny--;
        break;
    
    default:
        break;
    }
    char target, original;
    original=*(*((Vmap->map)+ox)+oy);
    if(checkBound(nx,ny,Vmap->rows,Vmap->cols)){
        target=*(*((Vmap->map)+nx)+ny);
        if(target!='T' && target!=']' && target!='['){
            thePlayer->x=nx; thePlayer->y=ny;
            int clr=0;
            if(isColour){
                if (original=='#') clr=BUSHCOLOUR;
                else if (original=='O') clr=PUDDLECOLOUR;
                
            }
            if(isColour) wattron(pwin, COLOR_PAIR(clr));
            mvwaddch(pwin,ox,oy,original);
            if(isColour) wattroff(pwin, COLOR_PAIR(clr));
            if(isColour) wattron(pwin, COLOR_PAIR(SELFCOLOUR));
            mvwaddch(pwin,nx,ny,'@');
            if(isColour) wattroff(pwin, COLOR_PAIR(SELFCOLOUR));
            handleDrop();
        }
        else if(target==']'){
            thePlayer->map +=1;
            thePlayer->x=Vmap->nx;
            thePlayer->y=Vmap->ny;
            freeVmap(Vmap);
            Vmap = constructVmap();
            werase(pwin);
            wrefresh(pwin);
            delwin(pwin);
            *oOverWorld = pwin=initOverWorld();
            wrefresh(pwin);

        }
        else if (target=='[')
        {
            thePlayer->map -=1;
            thePlayer->x=Vmap->px;
            thePlayer->y=Vmap->py;
            freeVmap(Vmap);
            Vmap = constructVmap();
            werase(pwin);
            wrefresh(pwin);
            delwin(pwin);
            *oOverWorld = pwin=initOverWorld();
            wrefresh(pwin);
        }
        napms(40);
        if(target=='#'){
            wrefresh(pwin);
            handleSpawn(pwin, theTerdex);
        }
        return;
    }
    return;
}

//HANDLES NEW SPAWN TERMON ON A MAP
void handleSpawn(WINDOW * win, dex * theTerdex){
    int hasPlayableMons=0;
    team * teamMate = thePlayer->pTeam;

    while((teamMate)){
        if(((teamMate->mon)->health)>0){
            hasPlayableMons=1;
            break;
        }
        teamMate=teamMate->next;
    }
    if(hasPlayableMons){
        if(rand()%SPAWN_STEPS==0){
            startBattle(win,theTerdex);
        }
    }
    return;
}

//HANDLES RANDOM ITEM DROPS ON A MAP
void handleDrop(){
    if((rand()%DROP_STEPS)==0){
        int items=0;
        bag * item = thePlayer->pBag;
        while(item){
            items++;
            item=item->next;
        }
        item = thePlayer->pBag;
        if(items==1){
            thePlayer->pBag->itemQuant +=1;
            return;
        }
        int chosen = rand() % (items);
        for(int i=0; i<chosen;i++){
            item=item->next;
        }
        item->itemQuant +=1;
    }
    return;
}

//UTILITIES
    //CHECKS BOUND OF NEW STEP
int checkBound(int x, int y, int r, int c){
    if(x>=0 && y>=0 && x<r && y<c) return 1;
    return 0;
}