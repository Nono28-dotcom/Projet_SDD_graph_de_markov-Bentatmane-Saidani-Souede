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
typedef struct s_tarjan_vertex{
    int id;
    int num;
    int numacc;
    int indic;
}t_tarjan_vertex;

t_tarjan_vertex *T;
typedef struct t_classe{
    char nom_classe[100];
    t_tarjan_vertex ** Tsommets;
    int capacite_c;
    int nbsommets;
}t_classe;
typedef struct t_partition{
    t_classe ** Tclasses;
    int capacite_p;
    int nbclasses;
}t_partition;
t_tarjan_vertex * init_vertex ( liste_adjacence adj){
    int n= adj.taille;
    t_tarjan_vertex * T=malloc(n*sizeof(t_tarjan_vertex));
    for (int i=0; i<n; i++){
        T[i].id=i+1;
        T[i].num= -1;
        T[i].numacc=-1;
        T[i].indic=0;
    }
    return T;
}
typedef struct s_cell {
    int value;
    struct s_cell *next;
}t_cell;
typedef struct s_list {
    t_cell * head;
}t_list;
typedef t_list t_stacklist;




#endif //MARKOV_H