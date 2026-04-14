#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "h/termon.h"
#include "h/ui.h"
#include "h/player.h"
#include "h/utilib.h"
#include "h/constants.h"

//iNITIALISES A NEW PLAYER OBJECT
player * newPlayer(int starter, dex * terdex){
    player * newPlayer = NULL;
    if(starter!=0){
        newPlayer = (player *)malloc(sizeof(player));
        newPlayer->map=1;
        newPlayer->x=1;
        newPlayer->y=1;
        newPlayer->savefile=0;
        newPlayer->pBag=loadItems();
        (newPlayer->pBag)->itemQuant=10;
        (newPlayer->pBag->next)->itemQuant=10;

        team * newTeam = newPlayer->pTeam=(team *)malloc(sizeof(team));
        newTeam->teamID=1;
        newTeam->mon=genNewTermon(terdex, starter, STARTER_LEVEL);
        newTeam->prev = newTeam->next=NULL;
    }


    return newPlayer;
}

//RETURNS AN EMPTY NEW BAG
bag * loadItems(){
    FILE * fpt = fopen("data/items.csv", "r");
    if(!fpt){
        endwin();
        exit(1);
    }
    bag * head = NULL;
    bag * cur=NULL;
    char line[256];
    for(int i=0;i<256;i++) line[i]='\0';
    while(fgets(line,256,fpt)){
        if(!head){
            head = (bag *)malloc(sizeof(bag));
            head->prev=NULL;
            head->next=NULL;
            head->index=atoi(strtok(line,","));
            strncpy(head->itemName,strtok(NULL,","),MAX_ITEM_NAME_LEN);
            head->itemQuant=0;
            strncpy(head->description,strtok(NULL,","),MAX_ITEM_DESC_LEN);
            *(head->description+strcspn(head->description,"\n"))='\0';
            cur=head;
        }
        else{
            cur->next=(bag *)malloc(sizeof(bag));
            cur->next->prev=cur;
            cur=cur->next;
            cur->next=NULL;
            cur->index=atoi(strtok(line,","));
            strncpy(cur->itemName,strtok(NULL,","),MAX_ITEM_NAME_LEN);
            cur->itemQuant=0;
            strncpy(cur->description,strtok(NULL,","),MAX_ITEM_DESC_LEN);
            *(cur->description+strcspn(cur->description,"\n"))='\0';   
        }    
    }
    fclose(fpt);
    return head;
}

//ADD AN INITIALISED MON TO THE TEAM OF thePlayer
void addMonTeam(termon * theMon){
    team * first = thePlayer->pTeam;
    team * nteam = (team *)malloc(sizeof(team));
    if(!first){
        thePlayer->pTeam=nteam;
        nteam->next=NULL;
        nteam->prev=NULL;
        nteam->teamID=1;
        nteam->mon=theMon;
        return;
    }
    while(first->next){
        first=first->next;
    }
    first->next=nteam;
    nteam->prev=first;
    nteam->next=NULL;
    nteam->teamID=(first->teamID)+1;
    nteam->mon=theMon;
    return;
}

//CHANGES POSITION OF MON IN TEAM OF thePlayer
void moveMonTeam(team * teamMate, int dir){
    if((dir==1 && (teamMate->prev)==NULL) || (dir==0 && (teamMate->next)==NULL) || (teamMate==NULL)){
        return;
    }
    team * before;
    team * after;
    if(dir==1){
        before=(teamMate->prev)->prev;
        after=(teamMate->next);
        if(after) after->prev=teamMate->prev;
        if(before) before->next=teamMate;
        teamMate->next=teamMate->prev;
        teamMate->prev=before;
        (teamMate->next)->prev=teamMate;
        (teamMate->next)->next=after;
        if(!before){
            thePlayer->pTeam=teamMate;
        }

    }
    else{
        moveMonTeam(teamMate->next,1);
    }
    reindexTeam();
    return;
}

//REMOVES THE CURRENT MON IN TEAM OF thePlayer
void rmTeamMon(team * teamMate, WINDOW * parent, team ** displayedMon){
    int x,y;
    getbegyx(parent,x,y);

    WINDOW * query = newwin(9, 18, x+4, y+2);
    keypad(query,TRUE);
    int ch;
    int choice=0;
    int releaseOffset = 2+(18-4-strlen("Release"))/2;
    int cannotOffset = 2+(18-4-strlen("Cannot Remove:"))/2;
    int nameOffset = 2+(18-4-strlen(teamMate->mon->name))/2;
    int nameOffsetQ = 2+(18-4-strlen(teamMate->mon->name)-1)/2;
    while (1){
        werase(query);
        if(isColour){
            wattron(query,COLOR_PAIR(BOXCOLOUR));
            box(query,0,0);
            wattroff(query,COLOR_PAIR(BOXCOLOUR));
            wattron(query,COLOR_PAIR(HEADCOLOUR));
            mvwprintw(query,1,releaseOffset, "Release");
            mvwprintw(query,2,nameOffsetQ, "%s?",(teamMate->mon)->name);
            wattroff(query,COLOR_PAIR(HEADCOLOUR));
            if(choice){
                wattron(query,COLOR_PAIR(WARNING));
                mvwprintw(query,4,4,"--> Yes");
                wattroff(query,COLOR_PAIR(WARNING));
                wattron(query,COLOR_PAIR(SAFE));
                mvwprintw(query,5,4,"   No");
                wattroff(query,COLOR_PAIR(SAFE));
            }
            else{
                wattron(query,COLOR_PAIR(WARNING));
                mvwprintw(query,4,4,"   Yes");
                wattroff(query,COLOR_PAIR(WARNING));
                wattron(query,COLOR_PAIR(SAFE));
                mvwprintw(query,5,4,"--> No");
                wattroff(query,COLOR_PAIR(SAFE));
            }
            wattron(query,COLOR_PAIR(FOOTCOLOUR));
            mvwprintw(query,7,2,"Use Arrows");
            wattroff(query,COLOR_PAIR(FOOTCOLOUR));
        }
        else{
            box(query,0,0);
            mvwprintw(query,1,releaseOffset, "Release");
            mvwprintw(query,2,nameOffsetQ, "%s?",(teamMate->mon)->name);
            if(choice){
                mvwprintw(query,4,4,"--> Yes");
                mvwprintw(query,5,4,"    No");
            }
            else{
                mvwprintw(query,4,4,"    Yes");
                mvwprintw(query,5,4,"--> No");
            }
            mvwprintw(query,7,2,"Use Arrows");
        }
        wrefresh(query);
        ch = wgetch(query);
        if(ch==KEY_DOWN && choice){
            choice=0;
        }
        if(ch==KEY_UP &&!(choice)){
            choice=1;
        }
        if(ch==KEY_ENTER || ch==10){
            break;
        }
    }
    if(choice){
        termon * freemon = teamMate->mon;
        if(!(((teamMate->prev)==NULL)&&((teamMate->next)==NULL))){
            if(!(teamMate->next)){
                *displayedMon=teamMate->prev;
            }
            else{
                *displayedMon=teamMate->next;
            }

            if((teamMate->prev)==NULL){
                (teamMate->next)->prev=NULL;
                thePlayer->pTeam=teamMate->next;
                    
            }
            else if((teamMate->next)==NULL){
                (teamMate->prev)->next=NULL;

            }
            else{
                (teamMate->prev)->next=teamMate->next;
                (teamMate->next)->prev=teamMate->prev;
            }
            free(teamMate->mon);
            free(teamMate);
            reindexTeam();
        }
        else{
            werase(query);
            if(isColour){
                wattron(query, COLOR_PAIR(BOXCOLOUR));
                box(query,0,0);
                wattroff(query, COLOR_PAIR(BOXCOLOUR));
                wattron(query, COLOR_PAIR(WARNING));
                mvwprintw(query,1,cannotOffset,"Cannot Remove:");
                mvwprintw(query,2,nameOffset,"%s",freemon->name);
                wattroff(query, COLOR_PAIR(WARNING));
                wattron(query, COLOR_PAIR(CONTENTCOLOUR));
                mvwprintw(query,4,2,"As it is the");
                mvwprintw(query,5,2,"Sole TeamMon");
                wattroff(query, COLOR_PAIR(CONTENTCOLOUR));
                wattron(query,COLOR_PAIR(FOOTCOLOUR));
                mvwprintw(query,7,2,"Press any key");
                wattroff(query,COLOR_PAIR(FOOTCOLOUR));
            }
            else{
                box(query,0,0);
                mvwprintw(query,1,cannotOffset,"Cannot Remove:");
                mvwprintw(query,2,nameOffset,"%s",freemon->name);
                mvwprintw(query,4,2,"As it is the");
                mvwprintw(query,5,2,"Sole TeamMon");
                mvwprintw(query,7,2,"Press any key");
            }
            wrefresh(query);
            ch = wgetch(query);
        }

    }

    delwin(query);
    wrefresh(parent);
    return;
}

//HEALS THE CURRENT MON IN TEAM OF thePlayer
void healTeamMon(team * teamMate, WINDOW * parent){
    int x,y;
    getbegyx(parent,x,y);
    if(thePlayer->pBag->itemQuant==0){
        WINDOW * query = newwin(5,16,x+6,y+3);
        curs_set(0);
        if(isColour){
            wattron(query,COLOR_PAIR(BOXCOLOUR));
            box(query,0,0);
            wattroff(query,COLOR_PAIR(BOXCOLOUR));
            wattron(query,COLOR_PAIR(WARNING));
            mvwprintw(query,2,2,"Out of Heal!");
            wattroff(query,COLOR_PAIR(WARNING));
        }
        else{
            box(query,0,0);
            mvwprintw(query,2,2,"Out of Heal!");
        }
        wrefresh(query);
        napms(1800);
        werase(query);
        wrefresh(query);
        delwin(query);
        return;
    }
    else if((teamMate->mon->health)==(teamMate->mon->hp)){
        WINDOW * query = newwin(5,16,x+6,y+3);
        curs_set(0);
        if(isColour){
            wattron(query,COLOR_PAIR(BOXCOLOUR));
            box(query,0,0);
            wattroff(query,COLOR_PAIR(BOXCOLOUR));
            wattron(query,COLOR_PAIR(SAFE));
            mvwprintw(query,2,2,"Full Health!");
            wattroff(query,COLOR_PAIR(SAFE));
        }
        else{
            box(query,0,0);
            mvwprintw(query,2,2,"Full Health!");
        }
        wrefresh(query);
        napms(1800);
        werase(query);
        wrefresh(query);
        delwin(query);
        return;
    }
    WINDOW * query = newwin(10,18,x+4,y+2);
    keypad(query,TRUE);
    curs_set(0);

    int choice=0;
    int ch;
    while(1){
        werase(query);
        if(isColour){
            wattron(query, COLOR_PAIR(BOXCOLOUR));
            box(query,0,0);
            wattroff(query, COLOR_PAIR(BOXCOLOUR));
            wattron(query, COLOR_PAIR(HEADCOLOUR));
            mvwprintw(query,1,2,"Heal %s?", teamMate->mon->name);
            wattroff(query, COLOR_PAIR(HEADCOLOUR));
            wattron(query, COLOR_PAIR(OPTIONCOLOUR));
            mvwprintw(query,3,4,"    Yes");
            mvwprintw(query,5,4,"    No");
            if(choice==0){
                mvwaddch(query,3,4,'-');mvwaddch(query,3,5,'-');mvwaddch(query,3,6,'>');
                mvwaddch(query,5,4,' ');mvwaddch(query,5,5,' ');mvwaddch(query,5,6,' ');
            }
            else if(choice==1){
                mvwaddch(query,5,4,'-');mvwaddch(query,5,5,'-');mvwaddch(query,5,6,'>');
                mvwaddch(query,3,4,' ');mvwaddch(query,3,5,' ');mvwaddch(query,3,6,' ');            
            }
            wattroff(query, COLOR_PAIR(OPTIONCOLOUR));
            wattron(query, COLOR_PAIR(FOOTCOLOUR));
            mvwprintw(query,7,2,"U/D- options");
            mvwprintw(query,8,2,"ENTER- Choose");
            wattroff(query, COLOR_PAIR(FOOTCOLOUR));
        }
        else{
            box(query,0,0);
            mvwprintw(query,1,2,"Heal %s?", teamMate->mon->name);
            mvwprintw(query,3,4,"    Yes");
            mvwprintw(query,5,4,"    No");
            if(choice==0){
                mvwaddch(query,3,4,'-');mvwaddch(query,3,5,'-');mvwaddch(query,3,6,'>');
                mvwaddch(query,5,4,' ');mvwaddch(query,5,5,' ');mvwaddch(query,5,6,' ');
            }
            else if(choice==1){
                mvwaddch(query,5,4,'-');mvwaddch(query,5,5,'-');mvwaddch(query,5,6,'>');
                mvwaddch(query,3,4,' ');mvwaddch(query,3,5,' ');mvwaddch(query,3,6,' ');            
            }
            mvwprintw(query,7,2,"U/D- options");
            mvwprintw(query,8,2,"ENTER- Choose");
        }
        wrefresh(query);        
        ch = wgetch(query);
        if((ch=='U' || ch== 'u') && choice==1) choice--;
        else if((ch=='D' || ch== 'd') && choice==0) choice++;
        else if(ch==10 || ch==KEY_ENTER){
            break;
        }
    }
    if(choice==0){
        (thePlayer->pBag->itemQuant)-=1;
        teamMate->mon->health=teamMate->mon->hp;
        wrefresh(parent);
    }
    werase(query);
    wrefresh(query);
    delwin(query);
    return;
}

//SHOWS THE TEAM WINDOW OF thePlayer
void showTeam(){
    team * teamMate = thePlayer->pTeam;
    termon * curMon;
    WINDOW * teamWin = newwin(16,22,1,(Vmap->cols)+1);
    keypad(teamWin,TRUE);
    int ch;
    while(1){
        curMon = teamMate->mon;
        werase(teamWin);
        if(isColour){
            wattron(teamWin, COLOR_PAIR(BOXCOLOUR));
            box(teamWin,0,0);
            wattroff(teamWin, COLOR_PAIR(BOXCOLOUR));
            wattron(teamWin, COLOR_PAIR(HEADCOLOUR));
            mvwprintw(teamWin,1,7,"The Team");
            wattroff(teamWin, COLOR_PAIR(HEADCOLOUR));
            wattron(teamWin, COLOR_PAIR(LOGOCOLOUR));
            mvwprintw(teamWin,3,3,"-->%s", curMon->name);
            mvwprintw(teamWin,5,2,"Team Position: %3d", teamMate->teamID);
            mvwprintw(teamWin,6,2,"Health: %3d/%d", curMon->health,curMon->hp);
            mvwprintw(teamWin,7,2,"Lvl: %d", curMon->lvl);
            mvwprintw(teamWin,8,2,"XP: %lu", curMon->xp);
            wattroff(teamWin, COLOR_PAIR(LOGOCOLOUR));
            wattron(teamWin, COLOR_PAIR(CONTENTCOLOUR));
            mvwprintw(teamWin,10,2,"HpI: %2d| HpS: %3d",curMon->hpI,curMon->hp);
            mvwprintw(teamWin,11,2,"AtI: %2d| AtS: %3d",curMon->atkI,curMon->atk);
            mvwprintw(teamWin,12,2,"DfI: %2d| DfS: %3d",curMon->defI,curMon->def);
            mvwprintw(teamWin,13,2,"SpI: %2d| SpS: %3d",curMon->spdI,curMon->spd);
            wattroff(teamWin, COLOR_PAIR(CONTENTCOLOUR));
        }
        else{
            box(teamWin,0,0);
            mvwprintw(teamWin,1,7,"The Team");
            mvwprintw(teamWin,3,3,"-->%s", curMon->name);
            mvwprintw(teamWin,5,2,"Team Position: %3d", teamMate->teamID);
            mvwprintw(teamWin,6,2,"Health: %3d/%d", curMon->health,curMon->hp);
            mvwprintw(teamWin,7,2,"Lvl: %d", curMon->lvl);
            mvwprintw(teamWin,8,2,"XP: %lu", curMon->xp);
            mvwprintw(teamWin,10,2,"HpI: %2d| HpS: %3d",curMon->hpI,curMon->hp);
            mvwprintw(teamWin,11,2,"AtI: %2d| AtS: %3d",curMon->atkI,curMon->atk);
            mvwprintw(teamWin,12,2,"DfI: %2d| DfS: %3d",curMon->defI,curMon->def);
            mvwprintw(teamWin,13,2,"SpI: %2d| SpS: %3d",curMon->spdI,curMon->spd);
        }
        wrefresh(teamWin);
        ch = wgetch(teamWin);
        if(ch==KEY_RIGHT){
            if(teamMate->next) teamMate=teamMate->next;
        }
        if(ch==KEY_LEFT){
            if(teamMate->prev) teamMate=teamMate->prev;
        }
        if(ch=='U' || ch=='u') moveMonTeam(teamMate,1);
        if(ch=='D' || ch=='d') moveMonTeam(teamMate,0);
        if(ch=='R' || ch=='r') rmTeamMon(teamMate,teamWin,&teamMate);
        if(ch=='H' || ch=='h') healTeamMon(teamMate,teamWin);
        if(ch=='O' || ch=='o'){
            WINDOW * optionsWin = newwin(10,18,5,(Vmap->cols)+3);
            keypad(optionsWin,TRUE);
            int ch;
            if(isColour){
                wattron(optionsWin,COLOR_PAIR(BOXCOLOUR));
                box(optionsWin,0,0);
                wattroff(optionsWin,COLOR_PAIR(BOXCOLOUR));
                wattron(optionsWin,COLOR_PAIR(HEADCOLOUR));
                mvwprintw(optionsWin,1,5,"Options!");
                wattroff(optionsWin,COLOR_PAIR(HEADCOLOUR));
                wattron(optionsWin,COLOR_PAIR(CONTENTCOLOUR));
                mvwprintw(optionsWin,3,2,"U: Move Up");
                mvwprintw(optionsWin,4,2,"D: Move Down");
                mvwprintw(optionsWin,5,2,"H: Heal");
                mvwprintw(optionsWin,6,2,"R: Release");
                wattroff(optionsWin,COLOR_PAIR(CONTENTCOLOUR));
                wattron(optionsWin,COLOR_PAIR(FOOTCOLOUR));
                mvwprintw(optionsWin,8,2,"Press a Key...");
                wattroff(optionsWin,COLOR_PAIR(FOOTCOLOUR));
            }
            else{
                box(optionsWin,0,0);
                mvwprintw(optionsWin,1,5,"Options!");
                mvwprintw(optionsWin,3,2,"U: Move Up");
                mvwprintw(optionsWin,4,2,"D: Move Down");
                mvwprintw(optionsWin,5,2,"H: Heal");
                mvwprintw(optionsWin,6,2,"R: Release");
                mvwprintw(optionsWin,8,2,"Press a Key...");
            }
            ch =wgetch(optionsWin);
            touchwin(teamWin);
            werase(optionsWin);
            wrefresh(optionsWin);
            delwin(optionsWin);
        }
        if(ch=='T' || ch=='t') break;
    }
    wclear(teamWin);
    wrefresh(teamWin);
    delwin(teamWin);
    return;
}

//SHOWS THE BAG WINDOW OF thePlayer
void showBag(){
    bag * theBag = thePlayer->pBag;
    WINDOW * bagWin = newwin(10,24,1,(Vmap->cols)+1);
    keypad(bagWin,TRUE);
    curs_set(0);
    int offset = 2+(24-4-strlen("The Bag"))/2;
    int ch;
    while (1){
        werase(bagWin);
        if(isColour){
            wattron(bagWin,COLOR_PAIR(BOXCOLOUR));
            box(bagWin,0,0);
            wattroff(bagWin,COLOR_PAIR(BOXCOLOUR));
            wattron(bagWin,COLOR_PAIR(HEADCOLOUR));
            mvwprintw(bagWin,1,offset,"The Bag");
            wattroff(bagWin,COLOR_PAIR(HEADCOLOUR));
            wattron(bagWin,COLOR_PAIR(CONTENTCOLOUR));
            mvwprintw(bagWin,3,2,"%s",theBag->itemName);
            mvwprintw(bagWin,4,2,"Quantity: %d",theBag->itemQuant);
            mvwprintw(bagWin,5,2,"%s",theBag->description);
            wattroff(bagWin,COLOR_PAIR(CONTENTCOLOUR));
            wattron(bagWin,COLOR_PAIR(FOOTCOLOUR));
            mvwprintw(bagWin,8,2,"B: Close Bag");
            wattroff(bagWin,COLOR_PAIR(FOOTCOLOUR));
        }
        else{
            box(bagWin,0,0);
            mvwprintw(bagWin,1,offset,"The Bag");
            mvwprintw(bagWin,3,2,"%s",theBag->itemName);
            mvwprintw(bagWin,4,2,"Quantity: %d",theBag->itemQuant);
            mvwprintw(bagWin,5,2,"%s",theBag->description);
            mvwprintw(bagWin,8,2,"B: Close Bag");
        }
        wrefresh(bagWin);

        ch =wgetch(bagWin);
        if(ch=='B' || ch=='b'){
            break;
        }
        else if(ch==KEY_LEFT || ch=='P' || ch=='p'){
            if(theBag->prev) theBag=theBag->prev;
        }
        else if(ch==KEY_RIGHT || ch=='N' || ch=='n'){
            if(theBag->next) theBag=theBag->next;
        } 
    }
    werase(bagWin);
    wrefresh(bagWin);
    delwin(bagWin);
    return;
}

//ADDS AN ITEM TO THE BAG, NEW NODE IF NOT ALREADY THERE
void addItem(int id, int quant){
    bag * newItem=NULL;
    if(thePlayer->pBag==NULL){
        newItem = thePlayer->pBag=(bag *)malloc(sizeof(bag));  
        newItem->prev=NULL;
    }
    else{
        newItem=thePlayer->pBag;
        while(newItem->next){
            newItem=newItem->next;
        }
        newItem->next=(bag*)malloc(sizeof(bag));
        newItem->next->prev=newItem;
        newItem=newItem->next;
    }
    newItem->next=NULL;
    newItem->index=id;
    newItem->itemQuant=quant;

    bag * catItem = theCatalogue;
    while(catItem!=NULL && catItem->index!=id){
        catItem=catItem->next;
    } 
    if((catItem!=NULL) && catItem->index==id){
        strncpy(newItem->itemName,catItem->itemName,10);
        strncpy(newItem->description,catItem->description,30);
    }
    return;

}

//UTILS
    //REINDEXES TEAM MEMBERS POSITION IN CASE OF CHANGE
void reindexTeam(){
    if((thePlayer)){
        if((thePlayer->pTeam)){
            team * cur = thePlayer->pTeam;
            int i=1;
            while(cur){
                cur->teamID=i;
                i++;
                cur=cur->next;
            }
        }
    }
}
    //FREE
void freeBag(){
    bag *cur = thePlayer->pBag;
    bag *next;
    while(cur){
        next=cur->next;
        free(cur);
        cur=next;
    }
    return;
}

void freeTeam(){
    team *cur = thePlayer->pTeam;
    team *next;
    while(cur){
        next=cur->next;
        free(cur->mon);
        free(cur);
        cur=next;
    }
    return;
}

void freePlayer(){
    freeTeam();
    freeBag();
    free(thePlayer);
}

//DEBUG
void printPlayerData(){
    printf("Map: %d X: %d Y: %d\n", thePlayer->map, thePlayer->x, thePlayer->y);
    printBagData(thePlayer->pBag);
    printTeamData(thePlayer->pTeam);
}

void printBagData(bag * theBag){
    if(!theBag){
        printf("Empty Bag\n");
        return;
    }
    bag * curItem=theBag;
    printf("The bag has the following items:\n");
    while(curItem){
        printf("ID: %d Name: %s Quantity: %d desc: %s\n", curItem->index, curItem->itemName, curItem->itemQuant,curItem->description);
        curItem=curItem->next;
    }
    return;
}

void printTeamData(team * theTeam){
    if(!theTeam){
        printf("Empty team\n");
        return;
    }

    team * curMon=theTeam;
    termon * mon=curMon->mon;

    printf("The team has the following mons:\n");
    while(curMon){
        
        printf("ID: %d\n", curMon->teamID);
        mon=curMon->mon;
        printf("id: %d, lvl: %d, name: %s, hp:%d hpI:%d, atk:%d atkI:%d, def:%d defI:%d, spd:%d spdI:%d, health:%d\n", mon->id, mon->lvl, mon->name, mon->hp, mon->hpI, mon->atk, mon->atkI, mon->def, mon->defI, mon->spd, mon->spdI, mon->health);
        curMon=curMon->next;
    }
    return;

}

void printTeamData2(team * theTeam){
    if(!theTeam){
        printf("Empty team\n");
        return;
    }

    team * curMon=theTeam;
    termon * mon=curMon->mon;

    while(curMon){
        mon=curMon->mon;
        printf("id: %d, lvl: %d, hp:%d, atk:%d, def:%d, spd:%d,\n", mon->id, mon->lvl, mon->hp, mon->atk, mon->def, mon->spd);
        curMon=curMon->next;
    }
    return;

}