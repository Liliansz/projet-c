// Je déclare qu'il s'agit de mon propre travail


#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>    // Pour la vitesse du jeu
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


void afficher(struct Serpent s, int fruit_x, int fruit_y, int score, int bloc_x[], int bloc_y[], int nb_bloc);
void ecran_accueil();
int ecran_game_over(int score, int meilleur_score);
int bloc(struct Serpent s, int bloc_x[], int bloc_y[], int nb_bloc);
void initialiser_couleurs();
int collision_mur(struct Serpent s);
int collision_corps(struct Serpent s);



int main(){
    initscr();
    curs_set(0);            // cache le curseur
    nodelay(stdscr, true);  // serpent qui bouge sans attendre une touche
    initialiser_couleurs();


    /*---------Variables du serpent ---------*/

    struct Serpent s;
    s.taille = 1;
    s.direction = 'd';
    s.x[0] = LARGEUR / 2;
    s.y[0] = HAUTEUR / 2;


    int meilleur_score = 0;
    int rejouer = 1;


    ecran_accueil();
    
    while(rejouer){
        
        int touche;
        int en_jeu = 1;     /* 1 = jeu en cours, 0 = game over */ 
        int score = 0;
        int bloc_x[50];
        int bloc_y[50];
        int nb_bloc = 0;
        int fruit;
        int bloc_valide;

        /* Position du fruit à l'intérieur des murs donc entre 1 et LARGEUR - 1 */
        int fruit_x = rand() % (LARGEUR - 1) + 1;
        int fruit_y = rand() % (HAUTEUR - 1) + 1;


        /* Position du serpent dans la zone de jeu*/
        s.taille = 1;
        s.direction = 'd';
        s.x[0] = LARGEUR / 2;
        s.y[0] = HAUTEUR / 2;


        /* Initialisation du générateur aléatoire */
        srand(time(NULL));


        /*--------- Boucle principale du jeu ---------*/

        while(en_jeu){      /* Continue tant que en_jeu vaut 1 */

            /*Lire la touche appuyer*/
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
            


            /* Déplacement du corps (chaque caractère suit le précédent) */
            for (int i = s.taille - 1; i > 0; i--) {
                s.x[i] = s.x[i-1];
                s.y[i] = s.y[i-1];
            }

            /*Déplacer le serpent en continu*/
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
                s.taille++ ;  /* le serpent grandit */
                score = score + 10;       /*on ajoute 10 au score*/

                /* Générer un nouveau fruit */
                do{
                    fruit = 1;
                    fruit_x = rand() % (LARGEUR - 1) + 1;   /* +1 pour eviter d'etre dans 0 et donc etre dans le mur */
                    fruit_y = rand() % (HAUTEUR - 1) + 1;   /* +1 pour eviter d etre dans 0 et donc etre dans le mur */
                    for(int i = 0; i < s.taille; i++){
                        if(fruit_x == s.x[i] && fruit_y == s.y[i]){
                            fruit = 0;
                        }
                    }
                    for(int j = 0; j < nb_bloc; j++){
                        if(fruit_x == bloc_x[j] && fruit_y == bloc_y[j]){
                            fruit = 0;
                        }
                    }
                } while(fruit == 0);


                // Création d'obstacle
                if(score >= 250 && nb_bloc < 50){
                do{
                    bloc_valide = 1;
                    bloc_x[nb_bloc] = rand() % (LARGEUR - 1) + 1;
                    bloc_y[nb_bloc] = rand() % (HAUTEUR - 1) + 1;
                    // Vérifier qu'il n'est pas sur le corps
                    for(int i = 0; i < s.taille; i++){
                        if(bloc_x[nb_bloc] == s.x[i] && bloc_y[nb_bloc] == s.y[i]){
                            bloc_valide = 0;
                        }
                    }
                    // Vérifier qu'il n'est pas sur un autre bloc
                    for(int j = 0; j < nb_bloc; j++){
                        if(bloc_x[nb_bloc] == bloc_x[j] && bloc_y[nb_bloc] == bloc_y[j]){
                            bloc_valide = 0;
                        }
                    }
                    // Vérifier qu'il n'est pas sur le fruit
                    if(bloc_x[nb_bloc] == fruit_x && bloc_y[nb_bloc] == fruit_y){
                        bloc_valide = 0;
                    }
                } while(bloc_valide == 0);
                nb_bloc++;
                }
            }


            /*Conditions de perte*/
            if(collision_mur(s) || collision_corps(s)){
              en_jeu = 0; /*game over*/
            }
             
            if(bloc(s, bloc_x, bloc_y, nb_bloc)){
                en_jeu = 0; /*game over*/
            }

            /*5. Affichage*/
            if(en_jeu){
                afficher(s, fruit_x, fruit_y, score, bloc_x, bloc_y, nb_bloc);

                // Vitesse du serpent
                if(s.direction == 'z' || s.direction == 's'){
                    usleep(200000); // Pour que le serpent soit plus lent a la verticale
                } else{
                    usleep(100000); /* Vitesse du jeu, on attend un peu avant chaque frame*/
                }
            }
        }


        // Garde le meilleur score entre les parties
        if(score > meilleur_score){
            meilleur_score = score;
        }

        rejouer = ecran_game_over(score, meilleur_score);
    }
        
    endwin();
    return 0;
}



/*--------- Définition de fonction ---------*/

void ecran_accueil(){
    clear();
    mvprintw(HAUTEUR / 2 - 2, LARGEUR / 2 - 5, "SNAKE");
    mvprintw(HAUTEUR / 2, LARGEUR / 2 - 12, "Appuyer sur espace pour jouer");
    mvprintw(HAUTEUR / 2 + 2, LARGEUR / 2 - 10, "Touches : Z Q S D pour bouger");
    mvprintw(HAUTEUR / 2 + 4, LARGEUR / 2 - 8, "X pour quitter");
    refresh();

    int touche;
    do {
        touche = getch();
    } 
    while (touche != ' ');     // Lire tant que tu n'appuie pas sur la touche entré (' ')
}

int ecran_game_over(int score, int meilleur_score){

    clear();
    mvprintw(HAUTEUR / 2 - 2, LARGEUR / 2 - 5, "GAME OVER");
    mvprintw(HAUTEUR / 2, LARGEUR / 2 - 8, "Score : %d", score);
    mvprintw(HAUTEUR / 2 + 1, LARGEUR / 2 - 8, "Meilleur score : %d", meilleur_score);
    mvprintw(HAUTEUR / 2 + 3, LARGEUR / 2 - 12, "Appuyer sur espace pour rejouer");
    mvprintw(HAUTEUR / 2 + 4, LARGEUR / 2 - 8, "X pour quitter");
    refresh();

    int touche;
    do {
        touche = getch();
    }
    while (touche != ' ' && touche != 'x');    // Attendre espace ou x

    if (touche == 'x'){
        return 0;   // revenir a l ecran d accueil
    }
    return 1;   // rejouer
}

void afficher(struct Serpent s, int fruit_x, int fruit_y, int score, int bloc_x[], int bloc_y[], int nb_bloc){
    clear();    // efface l'ancien affichage

    int i;
    int j; 

    /* Mur du haut et du bas */
    attron(COLOR_PAIR(3) | A_BOLD);
    for(i = 0; i < LARGEUR; i = i + 1){
        mvprintw(0, i, "#");        /* mur du haut */
        mvprintw(HAUTEUR, i, "#");  /* mur du bas */
        mvprintw(HAUTEUR, LARGEUR, "#");
    }

    /* Mur du gauche et de droite */
    for(i = 0; i < HAUTEUR; i = i + 1){
        mvprintw(i, 0, "#");        /* mur de gauche */
        mvprintw(i, LARGEUR, "#");  /* mur de droite */
    }
    attroff(COLOR_PAIR(3) | A_BOLD);


    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(fruit_y, fruit_x, "*");    // Affiche le fruit
    attroff(COLOR_PAIR(2) | A_BOLD);

    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(s.y[0], s.x[0], "O");    // On affiche le serpent
    for(i = 1; i < s.taille; i++){
        mvprintw(s.y[i], s.x[i], "o");
    }
    attroff(COLOR_PAIR(1) | A_BOLD);
    

    mvprintw(0, LARGEUR + 5, "Score : %d", score);  /*affiche le score en haut*/

    attron(COLOR_PAIR(4) | A_BOLD);
    for(j = 0; j < nb_bloc; j++){
        mvprintw(bloc_y[j], bloc_x[j], "X");
    }
    attroff(COLOR_PAIR(4) | A_BOLD);

    refresh();  // On envoie l'affichage à l'écran
}

int bloc(struct Serpent s, int bloc_x[], int bloc_y[], int nb_blocs){
    int i;
    for(i = 0; i < nb_blocs; i++){
        if(s.x[0] == bloc_x[i] && s.y[0] == bloc_y[i]){
            return 1; 
        }
    }
    return 0; 
}

void initialiser_couleurs(){
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);   // couleur du serpent
  init_pair(2, COLOR_WHITE, COLOR_BLACK);     // couleur du bonbon
  init_pair(3, COLOR_RED, COLOR_BLACK);     // couleur du murs
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // couleurs blocs 
}

int collision_mur(struct Serpent s){
  if(s.x[0] <= 0 || s.x[0] >= LARGEUR || s.y[0] <= 0 || s.y[0] >= HAUTEUR){
    return 1;     /* game over, il y a une collision */
  }
  return 0;
}

int collision_corps(struct Serpent s){
  int i;
  for (i = 1; i < s.taille; i++){
    if(s.x[0] == s.x[i] && s.y[0] == s.y[i]){
      return 1;
    }
  }
  return 0;
}
