#include <stdio.h>
#include <stdlib.h>

#include "h/overworld.h"
#include "h/saves.h"
#include "h/termon.h"
#include "h/ui.h"
#include "h/player.h"
#include "h/utilib.h"
#include "h/constants.h"

//RETURNS NUMBER OF SAVE FILES
int saveCount(){
    FILE * saveCount = fopen("saves/saveCount.txt", "r");
    if(saveCount==NULL){
        saveCount = fopen("saves/saveCount.txt","w");
        fprintf(saveCount,"0");
        fclose(saveCount);
        return 0;
    }
    else{
        char num[4];
        for(int i=0; i<4; i++) num[i]='\0';
        fgets(num,4,saveCount);
        fclose(saveCount);
        return atoi(num);
    }
}

//HANDLES NEW GAME FROM EITHER SAVEFILE OR FROM SCRATCH
player * handleNewGame(int mode, dex * terdex){
    player * Player = NULL;
    
    if(mode == 0){
        int starter=selectStarter();
        if(starter==-1) gexitd(DEBUG);
        Player = newPlayer(starter, terdex);
        return Player;
    }
    else{
        Player = (player*)malloc(sizeof(player));
        Player->pTeam=NULL;
        Player->pBag=NULL;
        Player->savefile=mode;

        char line[513];
        cleanString(line,513);

        char saveFileName[100];
        cleanString(saveFileName,100);
        sprintf(saveFileName,"saves/%d.tps",mode);

        sline * head, * cur;
        cur = head = (sline *)malloc(sizeof(sline));
        cleanString(head->contents,513);
        head->next=NULL;

        FILE * saveFile = fopen(saveFileName,"r");
        fgets(cur->contents,513,saveFile);
        rmnlString(cur->contents);
        while(fgets(line,513,saveFile)){
            cur->next=(sline *)malloc(sizeof(sline));
            cur=cur->next;
            cleanString(cur->contents,513);
            strncpy(cur->contents,line,513);
            rmnlString(cur->contents);
            cur->next=NULL;
            cleanString(line,513);
        }
        fclose(saveFile);

        int items=0;
        int mons=0;
        bag * curItem=Player->pBag;
        team * curTeam=Player->pTeam;
        termon * curMon=NULL;
        bag * trav=theCatalogue;
        dex * temp=NULL;
        int index=0;
        int quant=0;
        int id=0;
        int lvl=0;
        unsigned long xp=0;

        crypt(head);

        cur=head;
        while(cur){
            if(strncmp(cur->contents,"=",1)==0){
                if(strncmp(cur->contents,"==I",3)==0){
                    items=1; mons=0;
                }
                else if(strncmp(cur->contents,"==T",3)==0){
                    mons=1;items=0;
                }
            }
            else if(mons && !(items)){
                if(Player->pTeam==NULL){
                    curTeam=Player->pTeam=(team *)malloc(sizeof(team));
                    curTeam->teamID=1;
                    curTeam->prev=NULL;
                }
                else{
                    curTeam->next=(team *)malloc(sizeof(team));
                    curTeam->next->prev=curTeam;
                    curTeam=curTeam->next;
                    curTeam->teamID=(curTeam->prev->teamID) +1;
                }
                curTeam->next=NULL;
                curMon=curTeam->mon=(termon *)malloc(sizeof(termon));
                id=atoi(strtok(cur->contents,","));
                lvl=atoi(strtok(NULL,","));
                xp=strtoul(strtok(NULL,","),NULL,10);
                temp=nFromDex(terdex,id);
                curMon->id=id;
                curMon->lvl=lvl;
                curMon->xp=xp;
                strcpy(curMon->name,temp->name);
                curMon->health=atoi(strtok(NULL,","));
                curMon->hpI=atoi(strtok(NULL,","));
                curMon->atkI=atoi(strtok(NULL,","));
                curMon->defI=atoi(strtok(NULL,","));
                curMon->spdI=atoi(strtok(NULL,","));
                curMon->hp=genStat(temp->b_hp,lvl,curMon->hpI);
                curMon->atk=genStat(temp->b_atk,lvl,curMon->atkI);
                curMon->def=genStat(temp->b_def,lvl,curMon->defI);
                curMon->spd=genStat(temp->b_speed,lvl,curMon->spdI);               

            }
            else if(items && !(mons)){
                index = atoi(strtok(cur->contents,","));
                quant = atoi(strtok(NULL,","));
                if(Player->pBag==NULL){
                    curItem=Player->pBag=(bag *)malloc(sizeof(bag));
                    curItem->prev=NULL;
                }
                else{
                    curItem->next=(bag *)malloc(sizeof(bag));
                    curItem->next->prev=curItem;
                    curItem=curItem->next;
                }
                curItem->next=NULL;
                curItem->index=index;
                curItem->itemQuant=quant;
                while(trav && (trav->index!=index)) trav=trav->next;
                if(trav){
                    strncpy(curItem->itemName,trav->itemName,9);
                    strncpy(curItem->description,trav->description,29);
                    *(trav->itemName + 9)='\0';
                    *(trav->description + 29)='\0';
                }
                trav=theCatalogue;
            }
            else{
                Player->savefile=atoi(strtok(cur->contents,","));
                Player->map=atoi(strtok(NULL,","));
                Player->x=atoi(strtok(NULL,","));
                Player->y=atoi(strtok(NULL,","));
            }
            cur=cur->next;
        }
        /*printf("%s %d %d %d %d\n", saveFileName,Player->savefile,Player->map,Player->x,Player->y);
        printBagData(Player->pBag);
        printTeamData(Player->pTeam);*/
        cur = head;
        sline * next = cur->next;
        while(cur){
            free(cur);
            cur=next;
            if(next) next=next->next;
        }
    }
    return Player;
}

//UPDATES/GENERATES SAVEFILE FOR CURRENT GAME
void saveFile(){
    int saveNo=thePlayer->savefile;
    if(saveNo==0){
        saveNo=1+saveCount();
        thePlayer->savefile=saveNo;
        FILE * scf = fopen("saves/saveCount.txt","w");
        fprintf(scf,"%d\n", saveNo);
        fclose(scf);
    }

    sline * head, * curSL;
    curSL = head = (sline *)malloc(sizeof(sline));

    head->next=NULL;
    cleanString(curSL->contents,513);
    sprintf(head->contents,"%d,%d,%d,%d,", thePlayer->savefile,thePlayer->map,thePlayer->x,thePlayer->y);

    curSL->next = (sline *)malloc(sizeof(sline));
    curSL=curSL->next;
    cleanString(curSL->contents,513);
    sprintf(curSL->contents,"==ITEMS==");

    bag * item = thePlayer->pBag;
    while(item){
        curSL->next=(sline *)malloc(sizeof(sline));
        curSL=curSL->next;
        cleanString(curSL->contents,513);
        sprintf(curSL->contents,"%d,%d,",item->index,item->itemQuant);
        curSL->next=NULL;
        item=item->next;
    }

    curSL->next = (sline *)malloc(sizeof(sline));
    curSL=curSL->next;
    cleanString(curSL->contents,513);
    sprintf(curSL->contents,"==TEAM==");


    team * teamm = thePlayer->pTeam;
    while(teamm){
        curSL->next=(sline *)malloc(sizeof(sline));
        curSL=curSL->next;
        cleanString(curSL->contents,513);
        sprintf(curSL->contents,"%d,%d,%lu,%d,%d,%d,%d,%d,",teamm->mon->id,teamm->mon->lvl,teamm->mon->xp,teamm->mon->health,teamm->mon->hpI,teamm->mon->atkI,teamm->mon->defI,teamm->mon->spdI);
        curSL->next=NULL;
        teamm=teamm->next;
    }

    char saveFileName[100];
    cleanString(saveFileName,100);
    sprintf(saveFileName, "saves/%d.tps", saveNo);

    crypt(head);

    FILE * fpt = fopen(saveFileName,"w");
    curSL=head;
    while(curSL){
        fprintf(fpt,"%s\n",curSL->contents);
        curSL=curSL->next;
    }
    /* fprintf(fpt,"%d,%d,%d,%d,\n", thePlayer->savefile,thePlayer->map,thePlayer->x,thePlayer->y);
    fprintf(fpt,"==ITEMS==\n");
    bag * item = thePlayer->pBag;
    while(item){
        fprintf(fpt,"%d,%d,\n",item->index,item->itemQuant);
        item=item->next;
    }
    fprintf(fpt,"==TEAM==\n");
    team * teamm = thePlayer->pTeam;
    while(teamm){
        fprintf(fpt,"%d,%d,%lu,%d,%d,%d,%d,%d,\n",teamm->mon->id,teamm->mon->lvl,teamm->mon->xp,teamm->mon->health,teamm->mon->hpI,teamm->mon->atkI,teamm->mon->defI,teamm->mon->spdI);
        teamm=teamm->next;
    } */
    fclose(fpt);
    sline * cur=curSL=head;
    while(curSL){
        curSL=curSL->next;
        free(cur);
        cur=curSL;
    }
    
    return;
}