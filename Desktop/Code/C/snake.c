#include <stdio.h>
#include <ncurses.h>
#include <unistd>    //Pour la vitesse du jeu

int main(){
    initscr();
    curs_set(0);

    int x = 10, y = 10;
    int touche;
    char direction = 'd';

    while(1){
        touch = getch()

        if (touche == 'z'){
            direction = 'z';
        }
        else if (touche == 'd'){
            direction = 'd';
        }
        else if (touche == 's'){
            direction = 's';
        }
        else if (touche == 'q'){
            direction = 'q';
        }

        if(direction == 'z'){
            y = y - 1;
        }
        if(direction == 's'){
            y = y + 1;
        }
        if(direction == 'q'){
            x = x - 1;
        }
        if(direction == 'd'){
            x = x + 1;
        }

        clear();
        mvprintw(y, x, "0");
        refresh();
        usleep(100000);
    }
    
    endwin();
    return 0;
}