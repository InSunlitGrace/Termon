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


    int p1t=1;
    int p2t=1;
    int ch;
    int choice =1;

    WINDOW * teamSelect = newwin(18,60,1,1);
    keypad(teamSelect,TRUE);

    dex * template = nFromDex(availableDex,choice);
    char ** optionSprite = ascii(choice);

    int isSelecting=1;
    while(isSelecting){
        werase(teamSelect);
        if(isColour) wattron(teamSelect,COLOR_PAIR(BOXCOLOUR));
        box(teamSelect,0,0);
        if(isColour){
            wattroff(teamSelect,COLOR_PAIR(BOXCOLOUR));
            wattron(teamSelect,COLOR_PAIR(HEADCOLOUR));
        }
        mvwprintw(teamSelect,1,(getmaxx(teamSelect) - strlen("Player 1 Choose!")) /2,"Player %d Choose!",curPlayer);
        mvwprintw(teamSelect,3,2,"Teammate Number: %d", (curPlayer==1)?(p1t):(p2t));
        if(isColour){
            wattroff(teamSelect,COLOR_PAIR(HEADCOLOUR));
            wattron(teamSelect,COLOR_PAIR(CONTENTCOLOUR));
        }
        mvwprintw(teamSelect,5,2,"Termon: %s",template->name);
        mvwprintw(teamSelect,6,2,"ID: %d",choice);
        mvwprintw(teamSelect,7,2,"Level: %d",PVP_MON_LEVEL);
        if(isColour){
            wattroff(teamSelect,COLOR_PAIR(CONTENTCOLOUR));
            wattron(teamSelect,COLOR_PAIR(HEADCOLOUR));
        }
        mvwprintw(teamSelect,6,25,"Base Stats");
        mvwprintw(teamSelect,6,40,"Level Stats");
        if(isColour){
            wattroff(teamSelect,COLOR_PAIR(HEADCOLOUR));
            wattron(teamSelect,COLOR_PAIR(CONTENTCOLOUR));
        }
        mvwprintw(teamSelect,8,25,"HP:  %3d",template->b_hp);
        mvwprintw(teamSelect,9,25,"ATK: %3d", template->b_atk);
        mvwprintw(teamSelect,10,25,"DEF: %3d", template->b_def);
        mvwprintw(teamSelect,11,25,"SPD: %3d", template->b_speed);
        mvwprintw(teamSelect,13,25,"BST: %3d", template->b_hp+template->b_atk+template->b_def+template->b_speed);


        mvwprintw(teamSelect,8,40,"HP:  %3d", genStat(template->b_hp, PVP_MON_LEVEL, 31));
        mvwprintw(teamSelect,9,40,"ATK: %3d", genStat(template->b_atk, PVP_MON_LEVEL, 31));
        mvwprintw(teamSelect,10,40,"DEF: %3d", genStat(template->b_def, PVP_MON_LEVEL, 31));
        mvwprintw(teamSelect,11,40,"SPD: %3d", genStat(template->b_speed, PVP_MON_LEVEL, 31));
        mvwprintw(teamSelect,13,40,"BST: %3d", genStat(template->b_hp, PVP_MON_LEVEL, 31)+genStat(template->b_atk, PVP_MON_LEVEL, 31)+genStat(template->b_def, PVP_MON_LEVEL, 31)+genStat(template->b_speed, PVP_MON_LEVEL, 31));
        if(isColour){
            wattron(teamSelect,COLOR_PAIR(CONTENTCOLOUR));
            wattron(teamSelect,COLOR_PAIR(LOGOCOLOUR));
        }
        for(int i=0; i<4;i++){
            mvwprintw(teamSelect,9+i,4,"%s",optionSprite[i]);
        }
        if(isColour){
            wattroff(teamSelect,COLOR_PAIR(LOGOCOLOUR));
            wattron(teamSelect,COLOR_PAIR(FOOTCOLOUR));
        }
        mvwprintw(teamSelect,16,2,"Use UP/DOWN to Select, ENTER to Confirm.");
        if(isColour) wattroff(teamSelect,COLOR_PAIR(FOOTCOLOUR));
        wrefresh(teamSelect);
        napms(100);
        ch = wgetch(teamSelect);
        flushinp();
        if(ch==KEY_DOWN || ch=='d'){
            choice ++;
            if(choice>DEX_MONS_NUM) choice=1;
        }
        else if(ch==KEY_UP || ch=='u'){
            choice--;
            if(choice<1) choice = DEX_MONS_NUM;
        }
        else if(ch==10 || ch==KEY_ENTER){
            if(curPlayer==1 && p1t<=PVP_TEAM_SIZE){
                team * cur = p1->teamMate;
                while(cur->mon!=NULL) cur=cur->next;
                cur->mon=genPerfectNewTermon(availableDex,choice,PVP_MON_LEVEL);
                p1t++;
                curPlayer=2;
            }
            else if(curPlayer==2 && p2t<=PVP_TEAM_SIZE){
                team * cur = p2->teamMate;
                while(cur->mon!=NULL) cur=cur->next;
                cur->mon=genPerfectNewTermon(availableDex,choice,PVP_MON_LEVEL);
                p2t++;
                curPlayer=1;
            }
            if(p1t>PVP_TEAM_SIZE && p2t>PVP_TEAM_SIZE){
                isSelecting=0;
            }
            choice=1;
        }
        else if(ch=='q' || ch=='Q'){
            werase(teamSelect);
            wrefresh(teamSelect);
            delwin(teamSelect);
            endwin();
            freeDex(availableDex);
            freePlyr(p1);
            freePlyr(p2);
            exit(0);
        }
        template = nFromDex(availableDex,choice);
        freeSprite(optionSprite);
        optionSprite=ascii(choice);
    }
    werase(teamSelect);
    wrefresh(teamSelect);
    delwin(teamSelect);
    int isPlaying =1;
    WINDOW * matchWin = newwin(18,60,1,1);
    keypad(matchWin,TRUE);
    while(isPlaying){

    }
    /*team * iterator = p1->teamMate;
    while(iterator){
        termon * mon = iterator->mon;
        printf("%d %s %d %d %d %d %d\n", iterator->teamID, mon->name, mon->hp, mon->atk, mon->def, mon->spd, mon->id);
        iterator=iterator->next;
    }

    iterator = p2->teamMate;
    while(iterator){
        termon * mon = iterator->mon;
        printf("%d %s %d %d %d %d %d\n", iterator->teamID, mon->name, mon->hp, mon->atk, mon->def, mon->spd, mon->id);
        iterator=iterator->next;
    }*/
    freeDex(availableDex);
    freePlyr(p1);
    freePlyr(p2);
    exit(0);
    endwin();

    return;
}
