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

typedef struct {
  int identifiant;
  int numero;
  int numero_accessible;
  int dans_pile;
} t_tarjan_vertex;


typedef struct {
  char nom[10];
  t_tarjan_vertex *sommets;
  int nb_sommets;
  int capacite;
} t_classe;


typedef struct {
  t_classe *classes;
  int nb_classes;
  int capacite;
} t_partition;


typedef struct {
  int *elements;
  int sommet;
  int capacite;
} t_pile;


t_pile* creer_pile(int capacite);
int pile_vide(t_pile *p);
void empiler(t_pile *p, int valeur);
int depiler(t_pile *p);
void liberer_pile(t_pile *p);

t_partition* creer_partition();
void ajouter_classe(t_partition *part);
void ajouter_sommet_classe(t_classe *classe, t_tarjan_vertex sommet);
void afficher_partition(t_partition *part);
void liberer_partition(t_partition *part);

t_tarjan_vertex* initialiser_tarjan_vertices(liste_adjacence adj);
int min(int a, int b);
void parcours(int sommet_id, liste_adjacence adj, t_tarjan_vertex *vertices, t_pile *pile, t_partition *part, int *compteur);
t_partition* tarjan(liste_adjacence adj);



typedef struct {
    int classe_depart;
    int classe_arrivee;
} t_lien;

typedef struct {
    t_lien *liens;
    int nb_liens;
    int capacite;
} t_liens_array;


t_liens_array* creer_liens_array();
int lien_existe(t_liens_array *array, int depart, int arrivee);
void ajouter_lien(t_liens_array *array, int depart, int arrivee);
int* creer_tableau_sommet_vers_classe(t_partition *part, int nb_sommets);
t_liens_array* construire_hasse(liste_adjacence adj, t_partition *part);
int export_hasse_to_mermaid(t_partition *part, t_liens_array *liens, const char *filename);
void liberer_liens_array(t_liens_array *array);
int analyser_caracteristiques(t_partition *part, t_liens_array *liens);

#endif //MARKOV_H