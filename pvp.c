#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "h/player.h"
#include  "h/battle.h"
#include "h/termon.h"
#include "h/utilib.h"
#include "h/pvp.h"
#include"h/constants.h"

void freePlyr(plyr * thePlyr){
    team * curTeam = thePlyr->teamMate;
    while(curTeam->next){
        if(curTeam->mon!=NULL) free(curTeam->mon);
        curTeam=curTeam->next;
        free(curTeam->prev);
    }
    free(curTeam->mon);
    free(curTeam);
    free(thePlyr);
}

void pvpMatch(){
    flushinp();
    clear();
    refresh();
    int curPlayer = 1;
    plyr * p1 = (plyr *)malloc(sizeof(plyr));
    plyr * p2 = (plyr *)malloc(sizeof(plyr));
    p2->id =1 + (p1->id=1);

     team * curTeamMate, * curTeamMate2; 
    curTeamMate = p1->teamMate  = (team *)malloc(sizeof(team));
    curTeamMate2 = p2->teamMate = (team *)malloc(sizeof(team));
    curTeamMate->next=curTeamMate->prev=curTeamMate2->next=curTeamMate2->prev=NULL;
    curTeamMate->teamID=curTeamMate2->teamID=1;
    curTeamMate->mon=curTeamMate2->mon=NULL;

    for(int i=0;i<PVP_TEAM_SIZE-1; i++){
        curTeamMate->next=(team *)malloc(sizeof(team));
        curTeamMate2->next=(team *)malloc(sizeof(team));
        curTeamMate->next->prev=curTeamMate;
        curTeamMate2->next->prev=curTeamMate2;        
        curTeamMate=curTeamMate->next;
        curTeamMate2=curTeamMate2->next;
        curTeamMate->teamID=curTeamMate2->teamID=i+2;
        curTeamMate->mon=curTeamMate2->mon=NULL;
    }
    curTeamMate->next=curTeamMate2->next=NULL;
    
    dex * availableDex=genDex(DEX_FILE);


    int p1t=0;
    int p2t=0;
    int ch;
    int choice =1;

    WINDOW * teamSelect = newwin(20,60,1,1);
    keypad(teamSelect,TRUE);

    while(1){
        box(teamSelect,0,0);
        mvwprintw(teamSelect,1,(getmaxx(teamSelect) - strlen("Player 1 Choose!")) /2,"Player %d Choose!",curPlayer);
        mvwprintw(teamSelect,4,2,"Termon: %s",nFromDex(availableDex,choice)->name);
        mvwprintw(teamSelect,18,2,"Use UP/DOWN to Select, ENTER to Confirm.");
        wrefresh(teamSelect);
        napms(100);
        ch = wgetch(teamSelect);
        flushinp();
        if(ch==KEY_DOWN || ch=='d'){
            choice ++;
            if(choice>DEX_MONS_NUM) choice=1;
        }
        if(ch==KEY_UP || ch=='u'){
            choice--;
            if(choice<1) choice = DEX_MONS_NUM;
        }
        else if(ch==10 || ch==KEY_ENTER){
            if(curPlayer==1 && p2t!=PVP_TEAM_SIZE){
                curPlayer=2;
                p1t++;
            }
            else if(curPlayer=2 && p1t!=PVP_TEAM_SIZE){
                curPlayer=1;
                p2t++;
            }
            else ch='q';
        }
        if(ch=='q' || ch=='Q'){
            werase(teamSelect);
            wrefresh(teamSelect);
            delwin(teamSelect);
            endwin();
            freeDex(availableDex);
            freePlyr(p1);
            freePlyr(p2);

            exit(0);
        }
    }
    return;
}
