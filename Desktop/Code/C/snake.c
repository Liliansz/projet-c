#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>    //Pour la vitesse du jeu
#include <stdlib.h>
#include <time.h>      // Pour srand et time

#define LARGEUR 60
#define HAUTEUR 20

/*--------- Déclaration de fonction ---------*/

struct Serpent{
    int x[200];
    int y[200];
    int taille;
    char direction;
};


void afficher(struct Serpent s, int fruit_x, int fruit_y, int score);


int main(){
    initscr();
    curs_set(0);            //cache le curseur
    nodelay(stdscr, TRUE);  // serpent qui bouge sans attendre une touche


    /*---------Variables du serpent ---------*/

    int touche;
    int en_jeu = 1;     /* 1 = jeu en cours, 0 = game over */ 


    /* Initialisation du générateur aléatoire */
    srand(time(NULL));

    /* Position du fruit à l'intérieur des murs donc entre 1 et LARGEUR - 1 */
    int fruit_x = rand() % (LARGEUR - 1) + 1;
    int fruit_y = rand() % (HAUTEUR - 1) + 1;

    int score = 0;

    struct Serpent s;
    s.taille = 1;
    s.direction = 'd';
    s.x[0] = LARGEUR / 2;
    s.y[0] = HAUTEUR / 2;


    /*--------- Boucle principale du jeu ---------*/

    while(en_jeu){      /* Continue tant que en_jeu vaut 1 */

        /*1. Lire la touche appuyer*/
        touche = getch();


        /*2. Changer la direction quand tu appuies sur le bouton*/
        if (touche == 'z' && s.direction != 's'){
            s.direction = 'z';
        }
        else if (touche == 'd' && s.direction != 'q'){
            s.direction = 'd';
        }
        else if (touche == 's' && s.direction != 'z'){
            s.direction = 's';
        }
        else if (touche == 'q' && s.direction != 'd'){
            s.direction = 'q';
        }
        else if (touche == 'x'){
            en_jeu = 0;     /* quitter */
        }
        


        /* Déplacement du corps (chaque segment suit le précédent) */
        for (int i = s.taille - 1; i > 0; i--) {
            s.x[i] = s.x[i-1];
            s.y[i] = s.y[i-1];
        }

        /*3. Déplacer le serpent en continu*/
        if(s.direction == 'z'){
            s.y[0] = s.y[0] - 1;
        }
        if(s.direction == 's'){
            s.y[0] = s.y[0] + 1;
        }
        if(s.direction == 'q'){
            s.x[0] = s.x[0] - 1;
        }
        if(s.direction == 'd'){
            s.x[0] = s.x[0] + 1;
        }



        /* Le serpent mange le fruit ? */
        if(s.x[0] == fruit_x && s.y[0] == fruit_y){
            s.taille++;  /* le serpent grandit */
            score = score + 10;                     /*on ajoute 10 au score*/
            /* Générer un nouveau fruit */
            fruit_x = rand() % (LARGEUR - 1) + 1;   /* +1 pour eviter d'etre dans 0 et donc etre dans le mur */
            fruit_y = rand() % (HAUTEUR - 1) + 1;   /* +1 pour eviter d etre dans 0 et donc etre dans le mur */
        }


        /* 4. Collision sur le mur */
        if(s.x[0] <= 0 || s.x[0] >= LARGEUR || s.y[0] <= 0 || s.y[0] >= HAUTEUR){
            en_jeu = 0;     /* game over */
        }

        /*Collision avec soi-même*/
        for (int i = 1; i < s.taille; i++){
            if(s.x[0] == s.x[i] && s.y[0] == s.y[i]){
                en_jeu = 0;
            }
        }


        /*5. Afficher*/
        if(en_jeu){
            afficher(s, fruit_x, fruit_y, score);
            usleep(100000); /* Vitesse du jeu, on attend un peu avant chaque frame*/
        }
    }
    
    endwin();
    printf("Game over !\n");
    return 0;
}



/*--------- Définition de fonction ---------*/

void afficher(struct Serpent s, int fruit_x, int fruit_y, int score){
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
    
    mvprintw(fruit_y, fruit_x, "*");    // Affiche le fruit

    mvprintw(s.y[0], s.x[0], "O");    // On affiche le serpent
    for(i = 1; i < s.taille; i++){
        mvprintw(s.y[i], s.x[i], "o");
    }

    mvprintw(0, LARGEUR + 5, "Score : %d", score);  /*affiche le score en haut*/

    refresh();  // On envoie l'affichage à l'écran
}









/* Ce qu'il reste à faire : 
        - La difficulté(des blocs qui apparaissent à partir de 1000 en score)
        - Le disign
        - La possibilitée d'utiliser un joystique 
        - Tableau de tes meilleures scores qui s'affiche 
        - Faire un ecran d'affichage 
        - Poser le game over a meme le jeu et pas dans le terminale */
