#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>    //Pour la vitesse du jeu
#include <stdlib.h>
#include <time.h>      // Pour srand et time

#define LARGEUR 60
#define HAUTEUR 20

/*--------- Déclaration de fonction ---------*/

void afficher(int x, int y, int fruit_x, int fruit_y);



int main(){
    initscr();
    curs_set(1);    //cache le curseur
    nodelay(stdscr, TRUE);  // serpent qui bouge sans attendre une touche


    /*---------Variables du serpent ---------*/

    int x = LARGEUR / 2; 
    int y = HAUTEUR / 2;
    int touche;
    char direction = 'd';   //démarre vers la droite
    int en_jeu = 1;     /* 1 = jeu en cours, 0 = game over */ 

    /* Initialisation du générateur aléatoire */
    srand(time(NULL));

    /* Position du fruit à l'intérieur des murs donc entre 1 et LARGEUR - 1 */
    int fruit_x = rand() % (LARGEUR - 1) + 1;
    int fruit_y = rand() % (HAUTEUR - 1) + 1;


    /*--------- Boucle principale du jeu ---------*/

    while(en_jeu){      /* Continue tant que en_jeu vaut 1 */

        /*1. Lire la touche appuyer*/
        touche = getch();


        /*2. Changer la direction*/
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
        else if (touche == 'x'){
            en_jeu = 0;     /* quitter */
        }


        /*3. Déplacer le serpent*/
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


        /* Le serpent mange le fruit ? */
        if(x == fruit_x && y == fruit_y){
            /* Générer un nouveau fruit */
            fruit_x = rand() % (LARGEUR - 1) + 1;   /* +1 pour eviter d'etre dans 0 et dnc etre dans le mur */
            fruit_y = rand() % (HAUTEUR - 1) + 1;   /* +1 pour eviter d etre dans 0 et donc etre dans le mur */
        }


        /* 4. Collision sur le mur */
        if(x <= 0 || x >= LARGEUR || y <= 0 || y >= HAUTEUR){
            en_jeu = 0;     /* game over */
        }


        /*5. Afficher*/
        if(en_jeu){
            afficher(x, y, fruit_x, fruit_y);
            usleep(100000); /* Vitesse du jeu, on attend un peu avant chaque frame*/
        }
    }
    
    endwin();
    printf("Game over !\n");
    return 0;
}



/*--------- Définition de fonction ---------*/

void afficher(int x, int y, int fruit_x, int fruit_y){
    clear();    // efface l'ancien affichage

    int i;

    /* Mur du haut et du bas */
    for(i = 0; i < LARGEUR; i = i + 1){
        mvprintw(0, i, "#");        /* mur du haut */
        mvprintw(HAUTEUR, i, "#");  /* mur du bas */
    }

    /* Mur du gauche et de droite */
    for(i = 0; i < HAUTEUR; i = i + 1){
        mvprintw(i, 0, "#");        /* mur de gauche */
        mvprintw(i, LARGEUR, "#");  /* mur de droite */
    }

    mvprintw(fruit_y, fruit_x, "*");    // Afficher le fruit
    mvprintw(y, x, "O");    // On affiche le serpent
    refresh();  // On envoie l'affichage à l'écran
}









/* Ce qu'il reste à faire : 
        - Le corps du serpent qui se crée [tab x struct]
        - il ne peux pas faire d'aller-retours
        - Game over si le serpent se mord lui même
        - Le score
        - La difficulté(des blocs qui apparaissent à partir de 1000 en score)
        - Le disign
        - La possibilitée d'utiliser un joystique */