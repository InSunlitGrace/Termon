#include "h/battle.h"
#include "h/vcanvas.h"
#include "h/termon.h"
#include "h/player.h"
#include "h/utilib.h"

#include "h/constants.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#define BASE 1000

//INITIALISES A NEW BATTLE
void startBattle(WINDOW * parentWin, dex * theTerdex){
    flushinp();

    int spawnCount = Vmap->spawnCount;
    if(spawnCount==0) return;
    team * teamMon =thePlayer->pTeam;
    termon * battler = NULL;
    while(teamMon){
        if((teamMon->mon)->health > 0){
            battler=teamMon->mon;
            break;
        }
        teamMon=teamMon->next  ;
    }
    if(!battler) return;


    WINDOW * battleWin = newwin(16,40,1,1+Vmap->cols);
    if(!battleWin) return;
    keypad(battleWin, TRUE);
    curs_set(0); 

    termon * theSpawn = genNewTermon(theTerdex, *((Vmap->spawns)+(rand()%spawnCount)),(Vmap->baseSpawnLevel)-1+(rand()%3));
    char ** spawnSprite = ascii(theSpawn->id);
    char ** battlerSprite = ascii(battler->id);
    int spawnState=0; 

    int ch;
    while(1){
        werase(battleWin);
        if(isColour){
            wattron(battleWin,COLOR_PAIR(BOXCOLOUR));
            box(battleWin,0,0);
            wattroff(battleWin,COLOR_PAIR(BOXCOLOUR));
            wattron(battleWin,COLOR_PAIR(HEADCOLOUR));
            mvwprintw(battleWin,1,1+(40-(strlen("A Termon Appeared!")))/2,"%.*s",(int)strlen("A Termon Appeared!"),"A Termon Appeared!");
            wattroff(battleWin,COLOR_PAIR(HEADCOLOUR));
            wattron(battleWin,COLOR_PAIR(FOOTCOLOUR));
            mvwprintw(battleWin,13,2,"A-Attack C-Catch R-Run");
            mvwprintw(battleWin,14,2,"LEFT/RIGHT ARROW-Swap Mon");       
            wattroff(battleWin,COLOR_PAIR(FOOTCOLOUR));
            wattron(battleWin,COLOR_PAIR(CONTENTCOLOUR));
            mvwprintw(battleWin,3,2,"%s",theSpawn->name);
            mvwprintw(battleWin,4,2,"Lvl: %d",theSpawn->lvl);
            mvwprintw(battleWin,5,2,"HP: %d/%d",theSpawn->health,theSpawn->hp);
            mvwprintw(battleWin,8,2,"%s",battler->name);
            mvwprintw(battleWin,9,2,"Lvl: %d",battler->lvl);
            mvwprintw(battleWin,10,2,"HP: %d/%d",battler->health,battler->hp);
            mvwprintw(battleWin,11,2,"Team Slot: %d",teamMon->teamID);
            wattroff(battleWin,COLOR_PAIR(CONTENTCOLOUR));
            wattron(battleWin,COLOR_PAIR(LOGOCOLOUR));
            for(int i=0;i<4;i++){
                for(int j=0;j<8;j++){
                    mvwaddch(battleWin,3+i,30 +j,*(*(spawnSprite+i) +j));
                }
            }
            for(int i=0;i<4;i++){
                for(int j=0;j<8;j++){
                    mvwaddch(battleWin,8+i,30 +j,*(*(battlerSprite+i) +j));
                }
            }
            wattroff(battleWin,COLOR_PAIR(LOGOCOLOUR));
            if(spawnState!=0){
                wattron(battleWin,COLOR_PAIR((spawnState==1)?WARNING:SAFE));
                mvwprintw(battleWin,12,2,"Foe %s %s", theSpawn->name,(spawnState==1)?"fainted!":"caught!");
                wattroff(battleWin,COLOR_PAIR((spawnState==1)?WARNING:SAFE));
                wrefresh(battleWin);
                napms(2000);
                if(spawnState==1) free(theSpawn);
                flushinp();
                break;
            }
        }
        else{
            box(battleWin,0,0);
            mvwprintw(battleWin,1,1+(40-(strlen("A Termon Appeared!")))/2,"%.*s",(int)strlen("A Termon Appeared!"),"A Termon Appeared!");
            mvwprintw(battleWin,13,2,"A-Attack C-Catch R-Run");
            mvwprintw(battleWin,14,2,"LEFT/RIGHT ARROW-Swap Mon");            
            mvwprintw(battleWin,3,2,"%s",theSpawn->name);
            mvwprintw(battleWin,4,2,"Lvl: %d",theSpawn->lvl);
            mvwprintw(battleWin,5,2,"HP: %d/%d",theSpawn->health,theSpawn->hp);
            mvwprintw(battleWin,8,2,"%s",battler->name);
            mvwprintw(battleWin,9,2,"Lvl: %d",battler->lvl);
            mvwprintw(battleWin,10,2,"HP: %d/%d",battler->health,battler->hp);
            mvwprintw(battleWin,11,2,"Team Slot: %d",teamMon->teamID);
            for(int i=0;i<4;i++){
                for(int j=0;j<8;j++){
                    mvwaddch(battleWin,3+i,30 +j,*(*(spawnSprite+i) +j));
                }
            }
            for(int i=0;i<4;i++){
                for(int j=0;j<8;j++){
                    mvwaddch(battleWin,8+i,30 +j,*(*(battlerSprite+i) +j));
                }
            }
            if(spawnState!=0){
                mvwprintw(battleWin,12,2,"Foe %s %s", theSpawn->name,(spawnState==1)?"fainted!":"caught!");
                wrefresh(battleWin);
                napms(2000);
                if(spawnState==1){
                    free(theSpawn);
                }
                flushinp();
                break;
            }
        }
        wrefresh(battleWin);
        ch = wgetch(battleWin);
        if(ch=='r' || ch=='R'){
            free(theSpawn);
            break;
        }
        else if(ch=='c' || ch=='C'){
            int caught = tryCatch(theSpawn);
            if(caught==1){
                giveXP(battler,theSpawn);
                spawnState=-1;
            }
            else if(caught==-1){
                if(isColour){
                    wattron(battleWin,COLOR_PAIR(WARNING));
                    mvwprintw(battleWin,13,2,"No Nets Left!");
                    wattroff(battleWin,COLOR_PAIR(WARNING));
                }
                else{
                    mvwprintw(battleWin,13,2,"No Nets Left!");
                }
            }
            else if(caught==0){
                if(isColour){
                    wattron(battleWin,COLOR_PAIR(WARNING));
                    mvwprintw(battleWin,12,2,"%s Escaped!", theSpawn->name);
                    wattroff(battleWin,COLOR_PAIR(WARNING));
                }
                else{
                   mvwprintw(battleWin,12,2,"%s Escaped!", theSpawn->name);
                }
                wrefresh(battleWin);
                napms(1000);
                flushinp();
                battler->health -= damCalc(theSpawn,battler);
                if(isFainted(battler)){
                    int continueBattle=0;
                    teamMon = handleTeamFaint(battleWin, battler, &continueBattle);
                    if(continueBattle==0){
                        free(theSpawn);
                        break;
                    }
                    battler = teamMon->mon;
                    freeSprite(battlerSprite);
                    battlerSprite=ascii(battler->id);
                }
                continue;

            }
        }
        else if(ch=='a' || ch=='A'){
            int spawnSpeed = theSpawn->spd;
            int battlerSpeed = battler->spd;
            if(battlerSpeed>=spawnSpeed){
                theSpawn->health -= damCalc(battler,theSpawn);
                if(isFainted(theSpawn)){
                    giveXP(battler, theSpawn);
                    spawnState=1;
                    continue;
                }
                battler->health -= damCalc(theSpawn,battler);
                if(isFainted(battler)){
                    int continueBattle=0;
                    teamMon = handleTeamFaint(battleWin, battler, &continueBattle);
                    if(continueBattle==0){
                        free(theSpawn);
                        break;
                    }
                    battler = teamMon->mon;
                    freeSprite(battlerSprite);
                    battlerSprite=ascii(battler->id);
                }
            }
            else{
                battler->health -= damCalc(theSpawn,battler);
                if(isFainted(battler)){
                    int continueBattle=0;
                    teamMon = handleTeamFaint(battleWin, battler, &continueBattle);
                    if(continueBattle==0){
                        free(theSpawn);
                        break;
                    }
                    battler = teamMon->mon;
                    freeSprite(battlerSprite);
                    battlerSprite=ascii(battler->id);
                }
                theSpawn->health -= damCalc(battler,theSpawn);
                if(isFainted(theSpawn)){
                    giveXP(battler, theSpawn);
                    spawnState=1;
                    continue;
                }
            }
        }
        else if(ch=='p' || ch=='P' || ch==KEY_LEFT){
            team * previous = teamMon->prev;
            while(previous){
                if(previous->mon->health==0){
                    previous=previous->prev;
                }
                else{
                    break;
                }
            }
            if(previous){
                teamMon=previous;
                battler=teamMon->mon;
                freeSprite(battlerSprite);
                battlerSprite=ascii(battler->id);
                battler->health -= damCalc(theSpawn,battler);
                if(isFainted(battler)){
                    int continueBattle=0;
                    teamMon = handleTeamFaint(battleWin, battler, &continueBattle);
                    if(continueBattle==0){
                        free(theSpawn);
                        break;
                    }
                    battler = teamMon->mon;
                    freeSprite(battlerSprite);
                    battlerSprite=ascii(battler->id);
                }
            }
        }
        else if(ch=='n' || ch=='N' || ch==KEY_RIGHT){
            team * nexto = teamMon->next;
            while(nexto){
                if(nexto->mon->health==0){
                    nexto=nexto->next;
                }
                else{
                    break;
                }
            }
            if(nexto){
                teamMon=nexto;
                battler=teamMon->mon;
                freeSprite(battlerSprite);
                battlerSprite=ascii(battler->id);
                battler->health -= damCalc(theSpawn,battler);
                if(isFainted(battler)){
                    int continueBattle=0;
                    teamMon = handleTeamFaint(battleWin, battler, &continueBattle);
                    if(continueBattle==0){
                        free(theSpawn);
                        break;
                    }
                    battler = teamMon->mon;
                    freeSprite(battlerSprite);
                    battlerSprite=ascii(battler->id);
                
                }
            }
        }
    }
    freeSprite(spawnSprite);
    freeSprite(battlerSprite);
    werase(battleWin);
    wrefresh(battleWin);
    delwin(battleWin);
    touchwin(parentWin);
    wrefresh(parentWin);
    return;
}

//IF A TEAM MEMBER FAINTS
team * handleTeamFaint(WINDOW * parent, termon * theMon, int * confirm ){
    flushinp();
    WINDOW * faintWin;
    int allFaint=1;
    team * teamMate = thePlayer->pTeam;
    while (teamMate){
        if((teamMate->mon)->health >0){
            allFaint=0;
            break;
        }
        teamMate=teamMate->next;
    }
     
    if(allFaint){
        *confirm=0;
        faintWin = newwin(4,30, 7, 1+5+Vmap->cols);
        curs_set(0);
        if(isColour){
            wattron(faintWin,COLOR_PAIR(BOXCOLOUR));
            box(faintWin,0,0);
            wattroff(faintWin,COLOR_PAIR(BOXCOLOUR));
            wattron(faintWin,COLOR_PAIR(WARNING));
            mvwprintw(faintWin,1,2+(34-4-strlen(theMon->name)-strlen(" fainted!"))/2,"%s fainted!", theMon->name);
            mvwprintw(faintWin,2,2+(34-4- strlen("All Termon Fainted"))/2,"All Termon Fainted");
            wattroff(faintWin,COLOR_PAIR(WARNING));
        }
        else{
            box(faintWin,0,0);
            mvwprintw(faintWin,1,2+(34-4-strlen(theMon->name)-strlen(" fainted!"))/2,"%s fainted!", theMon->name);
            mvwprintw(faintWin,2,2+(34-4- strlen("All Termon Fainted"))/2,"All Termon Fainted");
        }
        wrefresh(faintWin);
        napms(2000);
        werase(faintWin);
        wrefresh(faintWin);
        delwin(faintWin);
        return NULL;

    }

    faintWin = newwin(10, 34, 4, 1+3+Vmap->cols);
    keypad(faintWin,TRUE);
    curs_set(0);
    nodelay(faintWin,TRUE);
    int ch;
    int option=0;
    team * trav = teamMate;
    while (1){
        werase(faintWin);
        if(isColour){
            wattron(faintWin,COLOR_PAIR(BOXCOLOUR));
            box(faintWin,0,0);
            wattroff(faintWin,COLOR_PAIR(BOXCOLOUR));
            wattron(faintWin,COLOR_PAIR(HEADCOLOUR));
            mvwprintw(faintWin,1,1,"%s fainted! Continue?", theMon->name);
            wattroff(faintWin,COLOR_PAIR(HEADCOLOUR));
            wattron(faintWin,COLOR_PAIR(FOOTCOLOUR));
            mvwprintw(faintWin,6,1,"UP/DOWN - Switch Option");
            mvwprintw(faintWin,7,1,"LEFT/RIGHT - Switch Mon");
            mvwprintw(faintWin,8,1,"Enter - Confirm Choice.");
            wattroff(faintWin,COLOR_PAIR(FOOTCOLOUR));
            wattron(faintWin,COLOR_PAIR(OPTIONCOLOUR));
            if(option==0){
                mvwprintw(faintWin,3,1,"--> %s %d/%d",(teamMate->mon->name),(teamMate->mon->health),(teamMate->mon->hp));
                mvwprintw(faintWin,4,1,"    End Battle");
            }
            else if(option==1){
                mvwprintw(faintWin,3,1,"    %s %d/%d",(teamMate->mon->name),(teamMate->mon->health),(teamMate->mon->hp));
                mvwprintw(faintWin,4,1,"--> End Battle");
            }
            wattroff(faintWin,COLOR_PAIR(OPTIONCOLOUR));
        }
        else{
            box(faintWin,0,0);
            mvwprintw(faintWin,1,1,"%s fainted! Continue?", theMon->name);
            mvwprintw(faintWin,6,1,"UP/DOWN - Switch Option");
            mvwprintw(faintWin,7,1,"LEFT/RIGHT - Switch Mon");
            mvwprintw(faintWin,8,1,"Enter - Confirm Choice.");
            if(option==0){
                mvwprintw(faintWin,3,1,"--> %s %d/%d",(teamMate->mon->name),(teamMate->mon->health),(teamMate->mon->hp));
                mvwprintw(faintWin,4,1,"    End Battle");
            }
            else if(option==1){
                mvwprintw(faintWin,3,1,"    %s %d/%d",(teamMate->mon->name),(teamMate->mon->health),(teamMate->mon->hp));
                mvwprintw(faintWin,4,1,"--> End Battle");
            }            
        }
        wrefresh(faintWin);
        ch = wgetch(faintWin);
        if(ch=='P' || ch=='p' || ch==KEY_LEFT){
            trav=trav->prev;
            while(trav){
                if((trav->mon)->health > 0){
                    teamMate=trav;
                    break;
                }
                trav=trav->prev;
            }
            if(!trav) trav=teamMate;
        }
        if(ch=='N' || ch=='n' || ch==KEY_RIGHT){
            trav=trav->next;
            while(trav){
                if((trav->mon)->health > 0){
                    teamMate=trav;
                    break;
                }
                trav=trav->next;
            }
            if(!trav) trav=teamMate;
        }
        if(ch=='D' || ch=='d' || ch==KEY_DOWN){
            option=1;
        }
        if(ch=='U' || ch=='u' || ch==KEY_UP){
            option=0;
        }        
        if(ch==10 || ch==KEY_ENTER){
            if(option==0){
                *confirm=1;
                return trav;
            }
            else if(option==1){
                *confirm=0;
                return NULL;
            }
        }
    }
    return NULL;
}

//UTILITIES
    //CALCULATES DAMAGE DONE TO DEFENDER BY ATTACKER
int damCalc(termon * attacker, termon * defender){
    int atk = attacker->atk;
    int def = defender->def;
    int lvl = attacker->lvl;
    int pwr=POWER;
    if(def<=0) def=1;
    int damage = (((((2*lvl)/5+2)*pwr*atk)/def)/50)+2;
    return damage;
}
    
    //CHECKS IF TERMON IS FAINTED
int isFainted(termon * mon){
    if(mon->health<=0){
        mon->health=0;
        return 1;
    }
    return 0;
}

    //GIVES XP IN CASE OF CATCH/FAINT OF WILD TERMON FOE
void giveXP(termon * winner, termon * loser){
    int L1 = winner->lvl;
    int L2 = loser->lvl;
    winner->xp += ((BASE)*L2)/L1;
    while((winner->xp)>=(genStartXP((winner->lvl)+1))){
        winner->lvl+=1;
        winner->health+=genStat((nFromDex(masterTerdex,winner->id))->b_hp,winner->lvl,winner->hpI)-winner->hp;
        winner->hp=genStat((nFromDex(masterTerdex,winner->id))->b_hp,winner->lvl,winner->hpI);
        winner->atk=genStat((nFromDex(masterTerdex,winner->id))->b_atk,winner->lvl,winner->atkI);
        winner->def=genStat((nFromDex(masterTerdex,winner->id))->b_def,winner->lvl,winner->defI);
        winner->spd=genStat((nFromDex(masterTerdex,winner->id))->b_speed,winner->lvl,winner->spdI);
    }
    return;
}

    //TRIES CATCHING A WILD TERMON SPAWN
int tryCatch( termon * theSpawn){
    if((thePlayer->pBag->next->itemQuant)==0){
        return -1;
    }
    else{
        double health=(double)theSpawn->health;
        double hp=(double)theSpawn->hp;
        double lvl=(double)theSpawn->lvl;
        double prob=0;
        if(health==1){
            prob=100/(1+lvl/50);
        }
        else{
            prob=100*(1-(health/hp))/(1+(lvl/10));
        }
        thePlayer->pBag->next->itemQuant -= 1;
        if(prob>=(1+rand()%100)){
            addMonTeam(theSpawn);
            return 1;
        }
        else{
            return 0;
        }
    }
}