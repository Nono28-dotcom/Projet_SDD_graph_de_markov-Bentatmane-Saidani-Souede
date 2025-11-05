//
// Created by nsoue on 01/11/2025.
//

#include "markov.h"
#include <stdio.h>
#include <stdlib.h>


cell* creat_cell(float prob, int arrivee){
  cell* new_cell = (cell*)malloc(sizeof(cell));
  if (new_cell == NULL){
    return NULL;
  }
  new_cell->proba=prob;
  new_cell->sommet_arrivee=arrivee;
  new_cell->next=NULL;
  return new_cell;
};


liste create_void_list() {
  liste l;
  l.head = NULL;
  return l;
}


void ajouter_cellule(liste *l, int arrivee, float prob) {
  cell *new_cell = creat_cell(prob, arrivee);
  new_cell->next = l->head;
  l->head = new_cell;
}


void print_list(liste l, int sommet) {
  printf("Liste pour le sommet %d : [head @]", sommet);
  cell *tmp = l.head;
  while (tmp != NULL) {
    printf(" -> (%d, %.2f)", tmp->sommet_arrivee, tmp->proba);
    tmp = tmp->next;
  }
  printf("\n");
}


liste_adjacence creer_liste_adjacence_vide(int taille) {
  liste_adjacence adj;
  adj.taille = taille;
  adj.tab = (liste*)malloc(taille * sizeof(liste));
  if (!adj.tab) {
    return NULL;
  }
  for (int i = 0; i < taille; i++)
    adj.tab[i] = create_void_list();
  return adj;
}


void afficher_liste_adjacence(liste_adjacence adj) {
  for (int i = 0; i < adj.taille; i++) {
    print_list(adj.tab[i], i + 1);
  }
}




