//
// Created by nsoue on 01/11/2025.
//

#ifndef MARKOV_H
#define MARKOV_H

#include <stdio.h>

typedef struct cell{
  int sommet_arrivee;
  float Proba;
  cell *next;
}cell;

typedef struct liste {
  cell *head;
}liste;

typedef struct liste_adjacence {
  liste *tab;
  int taille;
}liste_adjacence;


#endif //MARKOV_H
