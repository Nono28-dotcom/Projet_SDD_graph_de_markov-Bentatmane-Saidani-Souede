//
// Created by nsoue on 01/11/2025.
//

#include "markov.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hasse.h"

//la fonction create cell crée le type cellule en prenant en entrée ses champs prob (contenant la probabilité pour aller d'un sommet à un autre) et arrivee (contenant le sommet vers lequel la probabilité est dirigé)
//on assimile cet cellule aux arêtes du graph

cell* creat_cell(float prob, int arrivee) {
  cell* new_cell = (cell*)malloc(sizeof(cell));
  //on alloue l'espace dynamiquement
  if (new_cell == NULL){
    return NULL;
  }
  //lignes de sécurité au cas où la mémoire n'est pas allouée
  new_cell->proba=prob;
  new_cell->sommet_arrivee=arrivee;
  new_cell->next=NULL;
  //la cellule n'a pas besoin de champ next, on fixe alors la valeure a NULL
  return new_cell;
};

//la fonction create_void_list crée une liste vide qui servira plus tard a contenir l'ensemble des cellules

liste create_void_list() {
  liste l;
  l.head = NULL;
    //initialise le champ head à NULL
  return l;
}

//La fonction ajouter_cellule prends en entrée la liste et les valeurs des champs prob et arrivee afin d'insérer une cellule dans la liste

void ajouter_cellule(liste *l, int arrivee, float prob) {
  cell *new_cell = creat_cell(prob, arrivee);
  //injecte les champs prob et arrivé dans la cellule
  new_cell->next = l->head;
  //insère en tête de liste (le next deviens l'ancien head)
  l->head = new_cell;
  //on ajoute la cellule dans la liste
};

//afficher les arêtes sortantes d'un sommet

void print_list(liste l, int sommet) {
  printf("Liste pour le sommet %d : [head @]", sommet);
  cell *tmp = l.head;
  //initialise le pointeur de parcours
  while (tmp != NULL) {
    printf(" -> (%d, %.2f)", tmp->sommet_arrivee, tmp->proba);
    tmp = tmp->next;
    //parcours la liste et s'arrete lorsque next=NULL (la liste est finie)
  }
  printf("\n");
};

//la fonction creer_liste_adjacence_vide permet de creer une liste dont chaque case contiendra une sous liste des aretes sortantes d'u sommet
//On peut assimiler cette liste a une matrice de probabilité du graph

liste_adjacence creer_liste_adjacence_vide(int taille) {
  liste_adjacence adj;
  adj.taille = taille;
  adj.tab = (liste*)malloc(taille * sizeof(liste));
  //initialise la taille du tableau et alloue de l'espace pour celui ci en fonction de la taille
  if (!adj.tab) {
    exit(EXIT_FAILURE);
  }
  //sécurité au cas ou l'allocation échoue
  for (int i = 0; i < taille; i++)
    adj.tab[i] = create_void_list();
  return adj;
  //boucle pour insérer les liste dans la liste d'adjacence
};


//cette fonction afficher_liste_adjacence parcours la liste d'adjacence et l'affiche

void afficher_liste_adjacence(liste_adjacence adj) {
  for (int i = 0; i < adj.taille; i++) {
    print_list(adj.tab[i], i + 1);
  }
};

//la fonction readGraph permet de lire in fichier contenant un graph et de le reconstruire sur C

liste_adjacence readGraph (const char* filename) {
  FILE *file = fopen(filename, "rt");
  //ouvre le fichier en mode lecture texte
  int nbvert, depart, arrivee;
  float proba;
  if (file == NULL) {
    perror("Could not open file for reading");
    exit(EXIT_FAILURE);
  }
  //verifie l'existence du fichier
  if (fscanf(file, "%d", &nbvert) != 1){
    perror("Could not read number of vertices");
    exit(EXIT_FAILURE);
  }
  //verifie l'ouverture
  liste_adjacence adj = creer_liste_adjacence_vide(nbvert);
  //crée le tableau d'adjacence vide
  while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3){
    ajouter_cellule(&adj.tab[depart - 1], arrivee, proba);
  }
  //parcours du fichier et insertion dans la liste
  fclose(file);
  return adj;
};



//Cette fonction permet de vérifier si un graph est un graph de markov, donc de savoir si la somme de toutes les probas
//sortantes d'un sommet sont égales à 1

int verif_graphe_Markov(liste_adjacence adj) {
  int vrai = 1;
  //initialise la variable de "vérité" à vrai
  for (int i = 0; i < adj.taille; i++) {
    float cmp = 0.0;
    //initialise le compteur
    cell *tmp = adj.tab[i].head;
    //initialise un pointeur qui pointe sur le champ head de la sous liste
    while (tmp != NULL) {
      cmp += tmp->proba;
      tmp = tmp->next;
      //incrémente le compteur est passe à l'arête suivante
    }
    // parcours toute la liste d'adjacence, sous listes par sous listes, et somme la valeur des arrêtes avec un compteur
    if (cmp < 0.99 || cmp > 1.00) {
      printf("Le sommet %d n'est pas valide (P = %f)\n", i, cmp);
      vrai = 0;
    }
    //verifie si la somme est comprise entre 0.99 et 1, renvoie faux dans le cas contraire
  }
  if (vrai) {
    printf("Le graphe est un graphe de Markov.\n");
  } else {
    printf("Le graphe n'est pas un graphe de Markov.\n");
  }
  return vrai;
}


//la fonction export_to_mermaid prends un graph en entrée et l'exporte en format mermaid pour avoir une visualisation

int export_to_mermaid(const liste_adjacence *adj, const char *filename) {
  if (!adj || !filename)
    return 0;
  // verifie les pointeurs (liste d'adjacence et fichier)
  FILE *f = fopen(filename, "w");
  // ouvre le fichier en mode écriture
  if (!f) {
    perror("opening mermaid file"); return 0;
    // retourne une erreur au cas ou un problème a lieu lors de l'ouverture du fichier
  }

  fprintf(f, "---\n");
  fprintf(f, "config:\n");
  fprintf(f, "  layout: elk\n");
  fprintf(f, "  theme: neo\n");
  fprintf(f, "  look: neo\n");
  fprintf(f, "---\n");
  fprintf(f, "flowchart LR\n");
  //écrit le format pris en charge par mermaid dans le fichier (en tête mermaid)

  for (int i = 0; i < adj->taille; ++i) {
    char *id = getID(i + 1);
    //génére un identifiant mermaid unique
    fprintf(f, "%s((%d))\n", id, i + 1);
    //crée un sommet
  }
  fprintf(f, "\n");

  for (int i = 0; i < adj->taille; ++i) {
    cell *tmp = adj->tab[i].head;
    char *id_from = getID(i + 1);
    while (tmp != NULL) {
      char *id_to = getID(tmp->sommet_arrivee);
      fprintf(f, "%s -->|%.2f|%s\n", id_from, tmp->proba, id_to);
      tmp = tmp->next;
    }
  }
  // parcours la liste d'adjacence et écrit dans le fichier la valeur des arrêtes
  fclose(f);
  return 1;
}