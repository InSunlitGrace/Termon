#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int allFaintedPlyr(plyr * aPlyr){
    team * curTeam = aPlyr->teamMate;
    while(curTeam!=NULL){
        if(curTeam->mon->health >0) return 0;
        curTeam=curTeam->next;
    }
    return 1;
}

team * swapPlyrTeam(plyr * aPlyr){
    team * newTeamMate = aPlyr->teamMate;
    while(isFainted(newTeamMate->mon)){
        newTeamMate=newTeamMate->next;
    }
    team * temporary;
    WINDOW * swapWin = newwin(11,22,5,20);
    keypad(swapWin,TRUE);
    while(1){
        werase(swapWin);
        if(isColour) wattron(swapWin,COLOR_PAIR(BOXCOLOUR));
        box(swapWin,0,0);
        if(isColour){
            wattroff(swapWin,COLOR_PAIR(BOXCOLOUR));
            wattron(swapWin,COLOR_PAIR(HEADCOLOUR));
        }
        mvwprintw(swapWin,1,4,"Player %d Swap!", aPlyr->id);
        if(isColour){
            wattroff(swapWin,COLOR_PAIR(HEADCOLOUR));
            wattron(swapWin,COLOR_PAIR(OPTIONCOLOUR));
        }
        mvwprintw(swapWin,3,3,"--> %s", newTeamMate->mon->name);
        if(isColour){
            wattroff(swapWin,COLOR_PAIR(OPTIONCOLOUR));
            wattron(swapWin,COLOR_PAIR(CONTENTCOLOUR));
        }
        mvwprintw(swapWin,5,2,"HP:%3d/%3d ATK:%d", newTeamMate->mon->health,newTeamMate->mon->hp,newTeamMate->mon->atk);
        mvwprintw(swapWin,6,2,"DEF:%3d    SPD:%d", newTeamMate->mon->def,newTeamMate->mon->spd);
        if(isColour){
            wattroff(swapWin,COLOR_PAIR(CONTENTCOLOUR));
            wattron(swapWin,COLOR_PAIR(FOOTCOLOUR));
        }
        mvwprintw(swapWin,8,4,"Use LEFT/RIGHT");
        mvwprintw(swapWin, 9,3,"ENTER to Confirm");
        if(isColour) wattroff(swapWin,COLOR_PAIR(FOOTCOLOUR));
        wrefresh(swapWin);

        int ch;
        napms(100);
        ch =wgetch(swapWin);
        flushinp();

        if(ch == KEY_LEFT){
            temporary = newTeamMate->prev;
            while(temporary){
                if(!isFainted(temporary->mon)){
                    newTeamMate=temporary;
                    break;
                }
                else{
                    temporary=temporary->prev;
                }
            }
        }
        else if(ch==KEY_RIGHT){
            temporary = newTeamMate->next;
            while(temporary){
                if(!isFainted(temporary->mon)){
                    newTeamMate=temporary;
                    break;
                }
                else{
                    temporary=temporary->next;
                }
            }
        }
        else if(ch==10 || ch==KEY_ENTER){
            break;
        }
    }
    wclear(swapWin);
    delwin(swapWin);

    return newTeamMate;
}

void  endGame(int mode){
    WINDOW * overWin = newwin(11,22,5,20);
    keypad(overWin,TRUE);

    if(isColour) wattron(overWin, COLOR_PAIR(BOXCOLOUR));
    box(overWin,0,0);
    if(isColour){
        wattroff(overWin,COLOR_PAIR(BOXCOLOUR));
        wattron(overWin, COLOR_PAIR(HEADCOLOUR));
    }
    mvwprintw(overWin,1,6,"Game Over!");
    if(isColour){
        wattroff(overWin,COLOR_PAIR(HEADCOLOUR));
        wattron(overWin, COLOR_PAIR(CONTENTCOLOUR));
    }
    mvwprintw(overWin,3,3,"This Is Because:");
    if(mode<0){
        mvwprintw(overWin,4,7,"Player %d", -mode);
        mvwprintw(overWin,5,6,"FORFEITED!");
        mvwprintw(overWin,7,2,"At Least You Tried");
    }
    else{
        mvwprintw(overWin,4,7,"Player %d", (mode==1)?(2):1);
        mvwprintw(overWin,5,9,"WON!");
        mvwprintw(overWin,7,8,"Yayyy!");
    }
    if(isColour) wattroff(overWin,COLOR_PAIR(CONTENTCOLOUR));
    wrefresh(overWin);
    napms(2000);
    flushinp();
    if(isColour) wattron(overWin, COLOR_PAIR(FOOTCOLOUR));
    mvwprintw(overWin,9,2,"PRESS ANY KEY...");
    if(isColour) wattroff(overWin, COLOR_PAIR(FOOTCOLOUR));
    wrefresh(overWin);
    int ch;
    ch=wgetch(overWin);
    wclear(overWin);
    wrefresh(overWin);
    delwin(overWin);
    return;
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

    dex * template = NULL;
    char ** optionSprite = NULL;

    int isSelecting=1;
    while(isSelecting){
        template = nFromDex(availableDex,choice);
        optionSprite=ascii(choice);
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
            freeSprite(optionSprite);
            freePlyr(p1);
            freePlyr(p2);
            exit(0);
        }
        freeSprite(optionSprite);
        
    }
    freeDex(availableDex);
    werase(teamSelect);
    wrefresh(teamSelect);
    delwin(teamSelect);

    int isPlaying =1;
    curPlayer =1;
    int execFlag[3]={0,0,0};
    team * curTeamP1=p1->teamMate;
    team * curTeamP2=p2->teamMate;
    termon * curMonP1 = curTeamP1->mon;
    termon * curMonP2 = curTeamP2->mon;
    char ** monP1Ascii = ascii(curMonP1->id);
    char ** monP2Ascii = ascii(curMonP2->id);


    WINDOW * matchWin = newwin(18,40,1,11);
    keypad(matchWin,TRUE);
    while(isPlaying){
        werase(matchWin);
        if(isColour) wattron(matchWin,COLOR_PAIR(BOXCOLOUR));
        box(matchWin,0,0);
        if(isColour){
            wattroff(matchWin,COLOR_PAIR(BOXCOLOUR));
            wattron(matchWin,COLOR_PAIR(HEADCOLOUR));
        }
        mvwprintw(matchWin,1,(getmaxx(matchWin) - strlen("Match In Progress!")) /2,"Match In Progress!");
        mvwprintw(matchWin,4,2,"Player 1's Battler");
        mvwprintw(matchWin,9,2,"Player 2's Battler");
        if(isColour){
            wattroff(matchWin,COLOR_PAIR(HEADCOLOUR));
            wattron(matchWin, COLOR_PAIR(CONTENTCOLOUR));
        }
        mvwprintw(matchWin,2,(getmaxx(matchWin) - strlen("What will Player 1 do?")) /2,"What will Player %d do?", curPlayer);
        if(isColour){
            wattroff(matchWin,COLOR_PAIR(CONTENTCOLOUR));
            wattron(matchWin, COLOR_PAIR(OPTIONCOLOUR));
        }
        mvwprintw(matchWin,5,4,"%s",curMonP1->name);
        mvwprintw(matchWin,6,4,"HP: %3d/%3d",curMonP1->health,curMonP1->hp);
        mvwprintw(matchWin,10,4,"%s",curMonP2->name);
        mvwprintw(matchWin,11,4,"HP: %3d/%3d",curMonP2->health,curMonP2->hp);
        if(isColour){
            wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
            wattron(matchWin, COLOR_PAIR(LOGOCOLOUR));            
        }
        for(int i=0; i<MAX_TICO_ROWS; i++){
            mvwprintw(matchWin,4+i,30,"%8s", *(monP1Ascii +i));
            mvwprintw(matchWin,9+i,30,"%8s", *(monP2Ascii +i));
        }
        if(isColour){
            wattroff(matchWin, COLOR_PAIR(LOGOCOLOUR));
            wattron(matchWin,COLOR_PAIR(FOOTCOLOUR));
        }
        mvwprintw(matchWin, 16, 3, "A - Attack  S - Switch F - Forfeit");
        if(isColour) wattroff(matchWin,COLOR_PAIR(FOOTCOLOUR));
        wrefresh(matchWin);

        int dmg;

        if(execFlag[0]!=2){
            napms(100);
            ch = wgetch(matchWin);
            flushinp();
            if(ch== 'f' || ch=='F'){
                endGame(-curPlayer);
                touchwin(matchWin);
                wrefresh(matchWin);
                break;
            }
            else if(ch=='a' || ch=='A'){
                execFlag[curPlayer]=1;
                if(curPlayer==1) curPlayer=2;
                else curPlayer=1;
                execFlag[0]+=1;
            }
            else if(ch=='s' || ch=='S'){
                if(curPlayer==1){
                    curTeamMate=swapPlyrTeam(p1);
                    curMonP1=curTeamMate->mon;
                    freeSprite(monP1Ascii);
                    monP1Ascii=ascii(curMonP1->id);
                    curPlayer=2;
                }
                else if(curPlayer==2){
                    curTeamMate2=swapPlyrTeam(p2);
                    curMonP2=curTeamMate2->mon;
                    freeSprite(monP2Ascii);
                    monP2Ascii=ascii(curMonP2->id);
                    curPlayer=1;                    
                }
                touchwin(matchWin);
                wrefresh(matchWin);
                execFlag[0]+=1;
            }

        }
        else{
            if(execFlag[1]==1 && execFlag[2]==1){
                if(curMonP1->spd > curMonP2->spd){
                    curMonP2->health -= dmg = damCalc(curMonP1,curMonP2);
                    if(!isFainted(curMonP2)){
                        if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP2->name,dmg);
                        if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        wrefresh(matchWin);
                        mvwprintw(matchWin,15,1,"                    ");
                        napms(1000);

                        curMonP1->health -= dmg = damCalc(curMonP2,curMonP1);
                        if(isFainted(curMonP1)){
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP1->name);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);

                            if(allFaintedPlyr(p1)){
                                endGame(1);
                                touchwin(matchWin);
                                wrefresh(matchWin);
                                isPlaying=0;
                                break;
                            }
                            else{
                                curTeamMate=swapPlyrTeam(p1);
                                curMonP1=curTeamMate->mon;
                                freeSprite(monP1Ascii);
                                monP1Ascii=ascii(curMonP1->id);
                            }
                        }
                        else{
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP1->name,dmg);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);
                        }
                    }
                    else{
                        if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP2->name);
                        if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        wrefresh(matchWin);
                        mvwprintw(matchWin,15,1,"                    ");
                        napms(1000);

                        if(allFaintedPlyr(p2)){
                            endGame(2);
                            touchwin(matchWin);
                            wrefresh(matchWin);
                            isPlaying=0;
                            break;
                        }
                        else{
                            curTeamMate2=swapPlyrTeam(p2);
                            curMonP2=curTeamMate2->mon;
                            freeSprite(monP2Ascii);
                            monP2Ascii=ascii(curMonP2->id);
                        }
                    }

                }
                else if(curMonP1->spd < curMonP2->spd){
                    curMonP1->health -= dmg = damCalc(curMonP2,curMonP1);
                    if(!isFainted(curMonP1)){
                        if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP1->name,dmg);
                        if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        wrefresh(matchWin);
                        mvwprintw(matchWin,15,1,"                    ");
                        napms(1000);

                        curMonP2->health -= dmg = damCalc(curMonP1,curMonP2);
                        if(isFainted(curMonP2)){
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP2->name);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);

                            if(allFaintedPlyr(p2)){
                                endGame(2);
                                touchwin(matchWin);
                                wrefresh(matchWin);
                                isPlaying=0;
                                break;
                            }
                            else{
                                curTeamMate2=swapPlyrTeam(p2);
                                curMonP2=curTeamMate2->mon;
                                freeSprite(monP2Ascii);
                                monP2Ascii=ascii(curMonP2->id);
                            }
                        }
                        else{
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP2->name,dmg);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);
                        }
                    }
                    else{
                        if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP1->name);
                        if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        wrefresh(matchWin);
                        mvwprintw(matchWin,15,1,"                    ");
                        napms(1000);

                        if(allFaintedPlyr(p1)){
                            endGame(1);
                            touchwin(matchWin);
                            wrefresh(matchWin);
                            isPlaying=0;
                            break;
                        }
                        else{
                            curTeamMate=swapPlyrTeam(p1);
                            curMonP1=curTeamMate->mon;
                            freeSprite(monP1Ascii);
                            monP1Ascii=ascii(curMonP1->id);
                        }
                    }
                }
                else{
                    if(rand()%2==0){
                        curMonP2->health -= dmg = damCalc(curMonP1,curMonP2);
                        if(!isFainted(curMonP2)){
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP2->name,dmg);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);

                            curMonP1->health -= dmg = damCalc(curMonP2,curMonP1);
                            if(isFainted(curMonP1)){
                                if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP1->name);
                                if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                wrefresh(matchWin);
                                mvwprintw(matchWin,15,1,"                    ");
                                napms(1000);

                                if(allFaintedPlyr(p1)){
                                    endGame(1);
                                    touchwin(matchWin);
                                    wrefresh(matchWin);
                                    isPlaying=0;
                                    break;
                                }
                                else{
                                    curTeamMate=swapPlyrTeam(p1);
                                    curMonP1=curTeamMate->mon;
                                    freeSprite(monP1Ascii);
                                    monP1Ascii=ascii(curMonP1->id);
                                }
                            }
                            else{
                                if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP1->name,dmg);
                                if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                wrefresh(matchWin);
                                mvwprintw(matchWin,15,1,"                    ");
                                napms(1000);
                            }
                        }
                        else{
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP2->name);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);

                            if(allFaintedPlyr(p2)){
                                endGame(2);
                                touchwin(matchWin);
                                wrefresh(matchWin);
                                isPlaying=0;
                                break;
                            }
                            else{
                                curTeamMate2=swapPlyrTeam(p2);
                                curMonP2=curTeamMate2->mon;
                                freeSprite(monP2Ascii);
                                monP2Ascii=ascii(curMonP2->id);
                            }
                        }

                    }
                    else{
                        curMonP1->health -= dmg = damCalc(curMonP2,curMonP1);
                        if(!isFainted(curMonP1)){
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP1->name,dmg);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);

                            curMonP2->health -= dmg = damCalc(curMonP1,curMonP2);
                            if(isFainted(curMonP2)){
                                if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP2->name);
                                if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                wrefresh(matchWin);
                                mvwprintw(matchWin,15,1,"                    ");
                                napms(1000);

                                if(allFaintedPlyr(p2)){
                                    endGame(2);
                                    touchwin(matchWin);
                                    wrefresh(matchWin);                                    
                                    isPlaying=0;
                                    break;
                                }
                                else{
                                    curTeamMate2=swapPlyrTeam(p2);
                                    curMonP2=curTeamMate2->mon;
                                    freeSprite(monP2Ascii);
                                    monP2Ascii=ascii(curMonP2->id);
                                }
                            }
                            else{
                                if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP2->name,dmg);
                                if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                                wrefresh(matchWin);
                                mvwprintw(matchWin,15,1,"                    ");
                                napms(1000);
                            }
                        }
                        else{
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP1->name);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);
                            
                            if(allFaintedPlyr(p1)){
                                endGame(1);
                                touchwin(matchWin);
                                wrefresh(matchWin);
                                isPlaying=0;
                                break;
                            }
                            else{
                                curTeamMate=swapPlyrTeam(p1);
                                curMonP1=curTeamMate->mon;
                                freeSprite(monP1Ascii);
                                monP1Ascii=ascii(curMonP1->id);
                            }
                        }
                    }
                }
            }
            else{
                if(execFlag[1]==1){
                    curMonP2->health -= dmg = damCalc(curMonP1,curMonP2);
                    if(isFainted(curMonP2)){
                            if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP2->name);
                            if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                            wrefresh(matchWin);
                            mvwprintw(matchWin,15,1,"                    ");
                            napms(1000);

                        if(allFaintedPlyr(p2)){
                            endGame(2);
                            touchwin(matchWin);
                            wrefresh(matchWin);
                            isPlaying=0;
                            break;
                        }
                        else{
                            curTeamMate2=swapPlyrTeam(p2);
                            curMonP2=curTeamMate2->mon;
                            freeSprite(monP2Ascii);
                            monP2Ascii=ascii(curMonP2->id);
                        }
                    }
                    else{
                        if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP2->name,dmg);
                        if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        wrefresh(matchWin);
                        mvwprintw(matchWin,15,1,"                    ");
                        napms(1000);
                    }
                }
                else if(execFlag[2]==1){
                    curMonP1->health -= dmg = damCalc(curMonP2,curMonP1);
                    if(isFainted(curMonP1)){
                        if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        mvwprintw(matchWin,15,2,"%s FAINTED!",curMonP1->name);
                        if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        wrefresh(matchWin);
                        mvwprintw(matchWin,15,1,"                    ");
                        napms(1000);
                        
                        if(allFaintedPlyr(p1)){
                            endGame(1);
                            touchwin(matchWin);
                            wrefresh(matchWin);
                            isPlaying=0;
                            break;
                        }
                        else{
                            curTeamMate=swapPlyrTeam(p1);
                            curMonP1=curTeamMate->mon;
                            freeSprite(monP1Ascii);
                            monP1Ascii=ascii(curMonP1->id);
                        }
                    }
                    else{
                        if(isColour) wattron(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        mvwprintw(matchWin,15,2,"%s -%d HP!",curMonP1->name,dmg);
                        if(isColour) wattroff(matchWin,COLOR_PAIR(OPTIONCOLOUR));
                        wrefresh(matchWin);
                        mvwprintw(matchWin,15,1,"                    ");
                        napms(1000);
                    }
                }
            }
            execFlag[0]=execFlag[1]=execFlag[2]=0;
            wrefresh(matchWin);
            flushinp();
        }
    }
    werase(matchWin);
    wrefresh(matchWin);
    delwin(matchWin);
    
    freeSprite(monP1Ascii);
    freeSprite(monP2Ascii);
    freePlyr(p1);
    freePlyr(p2);
    endwin();
    exit(0);

    return;
}
