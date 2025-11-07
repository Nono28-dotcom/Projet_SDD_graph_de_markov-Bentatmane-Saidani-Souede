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
};


void ajouter_cellule(liste *l, int arrivee, float prob) {
  cell *new_cell = creat_cell(prob, arrivee);
  new_cell->next = l->head;
  l->head = new_cell;
};


void print_list(liste l, int sommet) {
  printf("Liste pour le sommet %d : [head @]", sommet);
  cell *tmp = l.head;
  while (tmp != NULL) {
    printf(" -> (%d, %.2f)", tmp->sommet_arrivee, tmp->proba);
    tmp = tmp->next;
  }
  printf("\n");
};


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
};


void afficher_liste_adjacence(liste_adjacence adj) {
  for (int i = 0; i < adj.taille; i++) {
    print_list(adj.tab[i], i + 1);
  }
};


liste_adjacence readGraph (const char* filename) {
  FILE *file = fopen(filename, "rt");
  int nbvert, depart, arrivee;
  float proba;
  if (file == NULL) {
    perror("Could not open file for reading");
    exit(EXIT_FAILURE);
  }
  if (fscanf(file, "%d", &nbvert) != 1){
    perror("Could not read number of vertices");
    exit(EXIT_FAILURE);
  }
  liste_adjacence adj = creer_liste_adjacence_vide(nbvert);
  while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3){
    ajouter_cellule(&adj.tab[depart - 1], arrivee, proba);
  }
  fclose(file);
  return adj;
};


void verif_graphe_Markov(liste_adjacence adj) {
  int vrai;
  vrai = 1;
  int cmp = 0;
  while (vrai == 1) {
    for (int i = 0; i < adj.taille; i++) {
      for (int j = 0; j < adj.taille; j++) {
        cmp = cmp + adj.tab[i][j].proba;
      }
      if (cmp < 0.99 || cmp > 1.00) {
        vrai = 0;
      }
      cmp = 0;
    }
  }
  if (vrai == 1){
    printf("%c", 'Le graphe est un graphe de Markov\n');
    return;
  }
  else {
    printf("%c", 'Le graphe n’est pas un graphe de Markov\n');
    return;
  }
}
void fichier_txt(t_list_adj *my_list_adj) {
    if (!my_list_adj || my_list_adj->size <= 0 || !my_list_adj->tab) {
        fprintf(stderr, "Graphe invalide.\n");
        return;
    }

    const char *filename = "test.txt";
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l’ouverture du fichier");
        return;
    }

    int size = my_list_adj->size;

    // Entête du fichier Mermaid
    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "  layout: elk\n");
    fprintf(file, "  theme: neo\n");
    fprintf(file, "  look: neo\n");
    fprintf(file, "---\n\n");
    fprintf(file, "flowchart LR\n\n");

    // Déclaration des sommets
    for (int i = 0; i < size; i++) {
        char id[10];
        strcpy(id, getID(i + 1));  // copier le buffer statique
        fprintf(file, "%s((%d))\n", id, i + 1);
    }
    fprintf(file, "\n");

    // Arêtes
    for (int i = 0; i < size; i++) {
        s_cell *curr = my_list_adj->tab[i].head;
        char from[10];
        strcpy(from, getID(i + 1));  // sommet de départ

        while (curr != NULL) {
            char to[10];
            strcpy(to, getID(curr->s_arrive));  // sommet d'arrivée réel
            fprintf(file, "%s -->|%.2f|%s\n", from, curr->s_proba, to);
            curr = curr->next;
        }
    }

    fclose(file);
}

//

