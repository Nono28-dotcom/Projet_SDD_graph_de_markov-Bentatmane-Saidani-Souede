//
// Created by nsoue on 01/11/2025.
//

#ifndef MARKOV_H
#define MARKOV_H
#include <stdio.h>
#include <stdlib.h>

typedef struct cell{
  int sommet_arrivee;
  float proba;
  struct cell *next;
}cell;

typedef struct liste {
  cell *head;
}liste;

typedef struct liste_adjacence {
  liste *tab;
  int taille;
}liste_adjacence;



cell* creat_cell(float prob, int arrivee);
liste create_void_list();
void ajouter_cellule(liste *l, int arrivee, float prob);
void print_list(liste l, int sommet);
liste_adjacence creer_liste_adjacence_vide(int taille);
void afficher_liste_adjacence(liste_adjacence adj);
liste_adjacence readGraph (const char* filename);
int verif_graphe_Markov(liste_adjacence adj);
int export_to_mermaid(const liste_adjacence *adj, const char *filename);



#endif //MARKOV_H
