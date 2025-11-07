//
// Created by nsoue on 01/11/2025.
//

#include "markov.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hasse.h"


cell* creat_cell(float prob, int arrivee) {
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
    exit(EXIT_FAILURE);
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


int verif_graphe_Markov(liste_adjacence adj) {
  int vrai;
  vrai = 1;
  for (int i = 0; i < adj.taille; i++) {
    float cmp = 0.0;
    cell *tmp = adj.tab[i].head;
    while (tmp != NULL) {
       cmp += tmp->proba;
       tmp = tmp->next;
    }
    if (cmp < 0.99 || cmp > 1.00) {
       printf("Le graphe n’est pas un graphe de Markov car P = %f \n", cmp);
       vrai = 0;
      }
    }
    printf("Le graphe est un graphe de Markov.");
    return vrai;
};


int export_to_mermaid(const liste_adjacence *adj, const char *filename) {
    if (!adj || !filename)
      return 0;
    FILE *f = fopen(filename, "w");
    if (!f) {
      perror("opening mermaid file"); return 0;
    }

    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "  layout: elk\n");
    fprintf(f, "  theme: neo\n");
    fprintf(f, "  look: neo\n");
    fprintf(f, "---\n");
    fprintf(f, "flowchart LR\n");

    for (int i = 0; i < adj->taille; ++i) {
        char *id = getID(i + 1);
        fprintf(f, "%s((%d))\n", id, i + 1);
        free(id);
    }
    fprintf(f, "\n");

    for (int i = 0; i < adj->taille; ++i) {
        cell *tmp = adj->tab[i].head;
        char *id_from = getID(i + 1);
        while (tmp != NULL) {
            char *id_to = getID(tmp->sommet_arrivee);
            fprintf(f, "%s -->|%.2f|%s\n", id_from, tmp->proba, id_to);
            free(id_to);
            tmp = tmp->next;
        }
        free(id_from);
    }

    fclose(f);
    return 1;
}
