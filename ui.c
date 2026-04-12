#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h/overworld.h"
#include "h/saves.h"
#include "h/termon.h"
#include "h/ui.h"
#include "h/player.h"
#include "h/utilib.h"

#include "h/constants.h"

#define WIN_ROWS 17
#define WIN_COLS 50

void drawFileInWindow(WINDOW *win, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        endwin();
        fprintf(stderr, "Error: Could not open %s\n", filename);
        exit(EXIT_FAILURE);
    }

    werase(win);     
    box(win, 0, 0);  

    char line[256];
    int row = 1; 
    while(fgets(line, sizeof(line), file))
    {
        line[strcspn(line,"\n")] = '\0';
        
        if(row >= WIN_ROWS-1) break; 
        mvwprintw(win, row, 1, "%.*s", WIN_COLS-2, line); 
        row++;
    }

    fclose(file);
    wrefresh(win); 
}
void dispLoad(){
    WINDOW *win = newwin(WIN_ROWS, WIN_COLS, 1, 1);
    keypad(win, TRUE);
    FILE *file = fopen(SPLASH, "r");
    if (!file)
    {
        endwin();
        fprintf(stderr, "Error: Could not open %s\n", SPLASH);
        exit(EXIT_FAILURE);
    }
    werase(win);
    if(isColour){
        wattron(win, COLOR_PAIR(BOXCOLOUR));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(BOXCOLOUR));
    }
    else{
        box(win,0,0);
    }

    char line[256];
    int row = 1; 
    while(fgets(line, sizeof(line), file))
    {
        line[strcspn(line,"\n")] = '\0';
        if(row == 1 && isColour) wattron(win,COLOR_PAIR(HEADCOLOUR));
        if(row == 7 && isColour) wattroff(win,COLOR_PAIR(HEADCOLOUR));
        if(row == 8 && isColour) wattron(win,COLOR_PAIR(LOGOCOLOUR));
        if(row == 11 && isColour) wattroff(win,COLOR_PAIR(LOGOCOLOUR));
        if(row == 12 && isColour) wattron(win,COLOR_PAIR(FOOTCOLOUR));
        if(row >= WIN_ROWS-1) break; 
        mvwprintw(win, row, 1, "%.*s", WIN_COLS-2, line); 
        row++;
    }
    fclose(file);
    if(isColour) wattroff(win,COLOR_PAIR(FOOTCOLOUR));
    wrefresh(win); 
    delwin(win);
}
void dispComand(){
    WINDOW *win = newwin(WIN_ROWS, WIN_COLS, 1, 1);
    keypad(win, TRUE);
    FILE *file = fopen(COMMANDS, "r");
    if (!file)
    {
        endwin();
        fprintf(stderr, "Error: Could not open %s\n", COMMANDS);
        exit(EXIT_FAILURE);
    }
    werase(win);
    if(isColour){
        wattron(win, COLOR_PAIR(BOXCOLOUR));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(BOXCOLOUR));
    }
    else{
        box(win,0,0);
    }
    char line[256];
    int row = 1; 
    while(fgets(line, sizeof(line), file))
    {
        line[strcspn(line,"\n")] = '\0';
        if(row == 1 && isColour) wattron(win,COLOR_PAIR(HEADCOLOUR));
        if(row == 2 && isColour) wattroff(win,COLOR_PAIR(HEADCOLOUR));
        if(row == 3 && isColour) wattron(win,COLOR_PAIR(CONTENTCOLOUR));
        if(row == 13 && isColour) wattroff(win,COLOR_PAIR(CONTENTCOLOUR));
        if(row == 14 && isColour) wattron(win,COLOR_PAIR(FOOTCOLOUR));
        if(row >= WIN_ROWS-1) break; 
        mvwprintw(win, row, 1, "%.*s", WIN_COLS-2, line); 
        row++;
    }
    fclose(file);
    if(isColour) wattroff(win,COLOR_PAIR(FOOTCOLOUR));
    wrefresh(win); 
    wgetch(win);
    delwin(win);
}
int showMenu(int numSaves) {
    int rows = 17, cols = 50;
    int starty = 1, startx = 1;

    WINDOW *win = newwin(rows, cols, starty, startx);
    keypad(win, TRUE);
    curs_set(0);

    int choice = 0; 
    int ch;
    int headOffset=2+((cols-4-strlen("CHOOSE THE GAME"))/2);
    int headOffset2=2+((cols-4-strlen("WHICH YOU WANT TO PLAY"))/2);
    int optionNewOffset1=((cols-4-strlen("--> New Game"))/2);
    int optionNewOffset2=((cols-4-strlen("   New Game"))/2);
    int optionSaveOffset1=((cols-4-strlen("--> New Game"))/2);
    int optionSaveOffset2=((cols-4-strlen("   New Game"))/2);
    while(1) {
        werase(win);
        if(isColour){
            wattron(win, COLOR_PAIR(BOXCOLOUR));
            box(win, 0, 0);
            wattroff(win, COLOR_PAIR(BOXCOLOUR));
            wattron(win, COLOR_PAIR(HEADCOLOUR));
            mvwprintw(win, 1, headOffset, "CHOOSE THE GAME");
            mvwprintw(win, 2, headOffset2, "WHICH YOU WANT TO PLAY");
            wattroff(win, COLOR_PAIR(HEADCOLOUR));
            wattron(win, COLOR_PAIR(OPTIONCOLOUR));
            if(numSaves>0) {
                mvwprintw(win, 6, (choice==0)?optionNewOffset1:optionNewOffset2, "%s New Game", (choice==0)?"-->":"  ");
                mvwprintw(win, 8, (choice>0)?optionSaveOffset1:optionSaveOffset2 , "%s SaveFile%d", (choice>0)?"-->":"  ", (choice>0)?choice:1);
            }
            else{
                mvwprintw(win, 7, (choice==0)?optionNewOffset1:optionNewOffset2, "%s New Game", (choice==0)?"-->":"  ");
            }
            wattroff(win, COLOR_PAIR(OPTIONCOLOUR));
            wattron(win, COLOR_PAIR(FOOTCOLOUR));
            mvwprintw(win, rows-2, 2, "UP/DOWN to scroll, Enter to select, Q to quit");
            wattroff(win, COLOR_PAIR(FOOTCOLOUR));

        }
        else{
            box(win,0,0);
            mvwprintw(win, 1, headOffset, "CHOOSE THE GAME");
            mvwprintw(win, 2, headOffset2, "WHICH YOU WANT TO PLAY");
            if(numSaves>0) {
                mvwprintw(win, 6, (choice==0)?optionNewOffset1:optionNewOffset2, "%s New Game", (choice==0)?"-->":"  ");
                mvwprintw(win, 8, (choice>0)?optionSaveOffset1:optionSaveOffset2 , "%s SaveFile%d", (choice>0)?"-->":"  ", (choice>0)?choice:1);
            }
            else{
                mvwprintw(win, 7, (choice==0)?optionNewOffset1:optionNewOffset2, "%s New Game", (choice==0)?"-->":"  ");
            }
            mvwprintw(win, rows-2, 2, "UP/DOWN to scroll, Enter to select, Q to quit");
        }
        wrefresh(win);

        ch = wgetch(win);
        if(ch==KEY_UP) {
            choice--;
            if(choice<0) choice = numSaves>0 ? numSaves : 0;
        }
        else if(ch==KEY_DOWN) {
            choice++;
            if(choice>numSaves) choice = 0;
        }
        else if(ch==10 || ch==KEY_ENTER) { 
            werase(win);
            wrefresh(win);
            delwin(win);
            return choice; 
        }
        else if(ch=='q' || ch=='Q') {
            delwin(win);
            endwin();
            exit(0);
        }
    }
}
int selectStarter() {
    int rows = 17, cols = 50;
    int starty = 1, startx = 1;

    WINDOW *win = newwin(rows, cols, starty, startx);
    keypad(win, TRUE);
    curs_set(0);

    const char *options[3] = {"Byteon", "Hexra", "Nibrix"};
    int choice = 0;
    char ** starterSprite = NULL;
    dex * template = NULL;

    int ch;
    int headOffset = 2+(cols-strlen("CHOOSE YOUR STARTING TERMON"))/2;
    while(1) {
        template=nFromDex(masterTerdex,choice+1);
        starterSprite = ascii(choice+1);
        werase(win);
        if(isColour){
            wattron(win, COLOR_PAIR(BOXCOLOUR));
            box(win, 0, 0);
            wattroff(win, COLOR_PAIR(BOXCOLOUR));
            wattron(win, COLOR_PAIR(HEADCOLOUR));
            mvwprintw(win, 1, headOffset, "CHOOSE YOUR STARTING TERMON");
            wattroff(win, COLOR_PAIR(HEADCOLOUR));
            wattron(win, COLOR_PAIR(OPTIONCOLOUR));
            for(int i=0;i<3;i++) {
                mvwprintw(win, 4+i*2, 3, "%s %s", (choice==i)?"-->":" ", options[i]);
            }
            wattroff(win, COLOR_PAIR(OPTIONCOLOUR));
            wattron(win, COLOR_PAIR(FOOTCOLOUR));
            mvwprintw(win, rows-2, 2, "Use UP/DOWN arrows, Enter to select, Q to quit");
            wattroff(win, COLOR_PAIR(FOOTCOLOUR));
            wattron(win,COLOR_PAIR(LOGOCOLOUR));
            for(int i=0;i<4;i++){
                for(int j=0;j<8;j++){
                    mvwaddch(win,4+i,28+j,*(*(starterSprite+i)+j));
                }
            }
            wattroff(win,COLOR_PAIR(LOGOCOLOUR));
            wattron(win,COLOR_PAIR(CONTENTCOLOUR));
            mvwprintw(win,9,26,"Base HP: %4d",template->b_hp);
            mvwprintw(win,10,26,"Base Atk:%4d",template->b_atk);
            mvwprintw(win,11,26,"Base Def:%4d",template->b_def);
            mvwprintw(win,12,26,"Base Spd:%4d",template->b_speed);
            mvwprintw(win,13,26,"BST:%9d",template->b_hp+template->b_atk+template->b_def+template->b_speed );
            wattroff(win,COLOR_PAIR(CONTENTCOLOUR));

        }
        else{
            box(win, 0, 0);
            mvwprintw(win, 1, headOffset, "CHOOSE YOUR STARTING TERMON");
            for(int i=0;i<3;i++) {
                mvwprintw(win, 4+i*2, 3, "%s %s", (choice==i)?"-->":" ", options[i]);
            }
            for(int i=0;i<4;i++){
                for(int j=0;j<8;j++){
                    mvwaddch(win,4+i,28+j,*(*(starterSprite+i)+j));
                }
            }
            mvwprintw(win,9,26,"Base HP: %4d",template->b_hp);
            mvwprintw(win,10,26,"Base Atk:%4d",template->b_atk);
            mvwprintw(win,11,26,"Base Def:%4d",template->b_def);
            mvwprintw(win,12,26,"Base Spd:%4d",template->b_speed);
            mvwprintw(win,13,26,"BST:%9d",template->b_hp+template->b_atk+template->b_def+template->b_speed );
            mvwprintw(win, rows-2, 2, "Use UP/DOWN arrows, Enter to select, Q to quit");
        }
        wrefresh(win);

        ch = wgetch(win);
        if(ch == KEY_UP) {
            choice--;
            if(choice<0) choice=2;
        }
        else if(ch == KEY_DOWN) {
            choice++;
            if(choice>2) choice=0; 
        }
        else if(ch==10 || ch==KEY_ENTER) {
            freeSprite(starterSprite);
            werase(win);
            wrefresh(win);
            delwin(win);
            return choice+1; 
        }
        else if(ch=='q' || ch=='Q') {
            freeSprite(starterSprite);
            werase(win);
            wrefresh(win);
            delwin(win);
            return -1;
        }
        freeSprite(starterSprite);
    }
}