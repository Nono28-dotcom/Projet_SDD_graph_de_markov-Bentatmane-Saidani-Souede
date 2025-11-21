//
// Created by nsoue on 21/11/2025.
//

#include "matrix.h"


//La fonction createEmptyMatrix permet de créeer une matrice pour contenir les resultats de la liste d'adjacence
//en d'autres termes, elle permet de créer la matrice probabilité d'un graph

t_matrix createEmptyMatrix(int n) {
    t_matrix M;
    M.rows = n;
    M.cols = n;
    //on crée la matrice carrée avec les lignes et les collones

    M.data = malloc(n * sizeof(float*));
    //on alloue l'espace necessaire pour la matrice entière
    for (int i = 0; i < n; i++) {
        M.data[i] = calloc(n, sizeof(float));
        //on alloue case par case l'espace necéssaire
    }
    return M;
}


//createMatrixFromAdjacency prends en entrée une liste d'adjacence et la met sous la forme d'une matrice de probabilité

t_matrix createMatrixFromAdjacency(liste_adjacence adj) {
    int n = adj.taille;
    t_matrix M = createEmptyMatrix(n);
    //initialise la taille et la matrice

    for (int i = 0; i < n; i++) {
        cell *tmp = adj.tab[i].head;
        //parcours la liste d'adjacence sous liste par sous liste

        while (tmp != NULL) {
            int j = tmp->sommet_arrivee - 1;
            M.data[i][j] = tmp->proba;
            tmp = tmp->next;
        }
    }

    return M;
}

//la fonction copyMatrix prends en entré deux matrice et copie les donnés

void copyMatrix(t_matrix dest, t_matrix src) {
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dest.data[i][j] = src.data[i][j];
            //parcours la matrice avec une double boucle(lignes, collones) et copie la matrice source dans la matrice destination
        }
    }
}


// la fonction multiplyMatrices prends deux matrices en entré

void multiplyMatrices(t_matrix A, t_matrix B, t_matrix result) {

    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {

            float sum = 0.0f;

            for (int k = 0; k < A.cols; k++) {
                sum += A.data[i][k] * B.data[k][j];
            }
            //effectue un produit matriciel pour chaque

            result.data[i][j] = sum;
            //stock le reésultat dans une matrice résultat
        }
    }
}






//la fonction freeMatrix libère l'espace alloué

void freeMatrix(t_matrix M) {
    for (int i = 0; i < M.rows; i++) {
        free(M.data[i]);
        //libere les cases
    }
    free(M.data);
    //libere la matrice
}


float diffMatrices(t_matrix A, t_matrix B) {
    float diff = 0.0f;

    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            diff += fabsf(A.data[i][j] - B.data[i][j]);
        }
    }
    return diff;
}