#include <stdio.h>
#include <string.h>

#define TAILLE_MAX 200

/* -----DEFINITION DES STRUCTURES-----*/
// Question 2a
struct etudiant {
    char prenom[50];
    char nom[50];
    int jour;
    int mois;
};

// Question 9
struct anniv {
    int jour;
    int mois;
    int nb_anniversaires;  //nbr de personnnes nées ce jour la 
};

/* ----- LES PROTOTYPES -----*/
void afficher_etudiant(struct etudiant e);
struct etudiant saisir_etudiant();
int cmp_etudiants(struct etudiant e, struct etudiant f);
int saisir_tab_etudiants(struct etudiant tab[], int taille_max);
void afficher_tab_etudiant(struct etudiant tab[], int taille);
void tri_insertion(struct etudiant tab[], int taille);
int anniversaires(struct etudiant tab[], int taille, struct anniv annivs[]);
int indice_max_annivs(struct anniv annivs[], int nb_annivs);


/* ----- LE MAIN -----*/
int main() {
    struct etudiant promo[TAILLE_MAX];
    struct anniv stats[TAILLE_MAX];
    
    // 1. Chargement des données
    int nb_etudiants = saisir_tab_etudiants(promo, TAILLE_MAX);
    printf("--- %d étudiants chargés ---\n", nb_etudiants);

    // 2. Tri
    tri_insertion(promo, nb_etudiants);
    printf("\n--- Liste triée par date d'anniversaire ---\n");
    afficher_tab_etudiant(promo, nb_etudiants);

    // 3. Statistiques
    int nb_dates = anniversaires(promo, nb_etudiants, stats);
    int imax = indice_max_annivs(stats, nb_dates);

    printf("\n--- Résultat ---\n");
    printf("Le jour avec le plus d'anniversaires est le %d/%d avec %d étudiants.\n", 
           stats[imax].jour, stats[imax].mois, stats[imax].nb_anniversaires);

    return 0;
}


/* ----- FONCTIONS -----*/
// Question 2c : Affichage d'un étudiant
void afficher_etudiant(struct etudiant e) {
    printf("%s %s, né(e) le %d/%d\n", e.nom, e.prenom, e.jour, e.mois);
}

// Question 2e : Saisie d'un étudiant
struct etudiant saisir_etudiant(){
  struct etudiant e;
  scanf("%s %s %d %d", e.nom, e.prenom, &e.jour, &e.mois);
  return e;
}

// Question 3 : Comparaison de deux dates d'anniversaire
int cmp_etudiants(struct etudiant e, struct etudiant f){
  if(e.mois < f.mois){
    return -1;
  }
  else if(e.mois > f.mois){
    return 1;
  }
  else {
    if(e.jour < f.jour){
      return -1;
    }
    if(e.jour > f.jour){
      return 1;
    }
    return 0;  //Meme jour, meme mois
  }
}

// Question 6 : Saisie d'un tableau (lecture depuis fichier/redirection)
int saisir_tab_etudiants(struct etudiant tab[], int taille_max) {
    int i = 0;
    // On lit le nom, puis le prénom, puis le format jj/mm
    while (i < taille_max && scanf("%s %s %d/%d", tab[i].nom, tab[i].prenom, &tab[i].jour, &tab[i].mois) == 4) {
        i++;
    }
    return i;
}

// Question 6 : Affichage du tableau
void afficher_tab_etudiant(struct etudiant tab[], int taille) {
    for (int i = 0; i < taille; i++) {
        afficher_etudiant(tab[i]);
    }
}

// Question 8 : Tri par insertion basé sur la date d'anniversaire
void tri_insertion(struct etudiant tab[], int taille) {
    for (int i = 1; i < taille; i++) {
        struct etudiant memoire = tab[i];  //pour stocker temporairement l etudiant qu on deplace
        int j = i;
        while (j > 0 && cmp_etudiants(tab[j - 1], memoire) > 0) {  //tant qu on est pas au debut du tableau ET que la pos j est plus grand que celui en memoire
            tab[j] = tab[j - 1];  //on decale vers la droite
            j--;
        }
        tab[j] = memoire;  //on range l etudiant a sa place
    }
}

// Question 10 : Calcul des occurrences d'anniversaires (sur tableau trié)
int anniversaires(struct etudiant tab[], int taille, struct anniv annivs[]) {
    if (taille == 0) return 0;
    
    int nb_jours_uniques = 0;
    for (int i = 0; i < taille; i++) {
        if (i > 0 && tab[i].jour == tab[i-1].jour && tab[i].mois == tab[i-1].mois) {
            // Même date que le précédent, on incrémente le compteur
            annivs[nb_jours_uniques - 1].nb_anniversaires++;
        } else {
            // Nouvelle date
            annivs[nb_jours_uniques].jour = tab[i].jour;
            annivs[nb_jours_uniques].mois = tab[i].mois;
            annivs[nb_jours_uniques].nb_anniversaires = 1;
            nb_jours_uniques++;
        }
    }
    return nb_jours_uniques;
}

// Question 11 : Trouver le jour avec le plus d'anniversaires
int indice_max_annivs(struct anniv annivs[], int nb_annivs) {
    int i_max = 0;  //on part du principe que le maximum est au debut 
    for (int i = 1; i < nb_annivs; i++) {
        if (annivs[i].nb_anniversaires > annivs[i_max].nb_anniversaires) {  //si on trouve un jour avec plus d anniversaires que notre max actuel
            i_max = i;  //on met a jour l indice du gagnant
        }
    }
    return i_max;
}
