//
// Created by nsoue on 21/11/2025.
//

#include "matrix.h"
#include "markov.h"
#include <math.h>

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
            int j = tmp->sommet_arrivee ;
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


//La fonction permet de calculer la différence entre deux matrices en sommant la valeur absolue de la différence des deux matrices

float diffMatrices(t_matrix A, t_matrix B) {
    float diff = 0.0f;

    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            diff += fabsf(A.data[i][j] - B.data[i][j]);
            //parcours les deux matrices et incrémente le compteur de différence
            //fabsf calcule la valeur absolue d'un float pris en argument
        }
    }
    return diff;
}

//La fonction afficherMatrix permet un affichage de la matrice dans le terminal

void afficherMatrix(t_matrix M) {
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.cols; j++) {
            printf("%.2f ", M.data[i][j]);
            //Parcours la structure matrice et affiche chaque élément de cette derniere
        }
        printf("\n");
    }
}


t_matrix subMatrix(t_matrix M, t_partition *part, int c) {
    int k = part->classes[c].nb_sommets;

    t_matrix S = createEmptyMatrix(k);
    int *indices = malloc(k * sizeof(int));

    // Récupération CORRECTE : utiliser .numero et non .identifiant
    for (int i = 0; i < k; i++) {
        indices[i] = part->classes[c].sommets[i].identifiant;

        if (indices[i] < 0 || indices[i] >= M.rows) {
            printf("ERREUR: sommet %d invalide dans classe %d\n",
                   part->classes[c].sommets[i].numero, c);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            S.data[i][j] = M.data[ indices[i] ][ indices[j] ];

    free(indices);
    return S;
}


t_matrix powerUntilLimit(t_matrix S, float eps, int max_iter) {
    int n = S.rows;

    t_matrix Mk  = createEmptyMatrix(n);
    t_matrix Mk1 = createEmptyMatrix(n);

    copyMatrix(Mk, S);

    for (int k = 1; k <= max_iter; k++) {
        multiplyMatrices(Mk, S, Mk1);

        float d = diffMatrices(Mk, Mk1);
        float d2 = ((int)(d * 100 + 0.5)) / 100.0f;

        if (d2 < eps) {
            freeMatrix(Mk);
            return Mk1;
        }

        copyMatrix(Mk, Mk1);
    }

    return Mk1;
}


void printStationaryForAllClasses(liste_adjacence g, t_partition *part) {
    t_matrix M = createMatrixFromAdjacency(g);

    printf("\n===== ÉTAPE 2 : Distributions stationnaires par classe =====\n");

    for (int c = 0; c < part->nb_classes; c++) {

        printf("\nClasse %d (taille %d)\n",
               c, part->classes[c].nb_sommets);

        t_matrix S = subMatrix(M, part, c);

        printf("\nSous-matrice S :\n \n");
        afficherMatrix(S);

        t_matrix L = powerUntilLimit(S, 0.01f, 50);

        printf("\nMatrice limite S exposant oo :\n \n");
        afficherMatrix(L);

        int k = L.rows;
        float somme = 0;
        for (int j = 0; j < k; j++)
            somme += L.data[0][j];

        printf("\nDistribution stationnaire : [");
        if (somme < 1e-6) {
            for (int j = 0; j < k; j++) printf("0 ");
        } else {
            for (int j = 0; j < k; j++)
                printf("%.3f ", L.data[0][j] / somme);
        }
        printf("]\n");

        freeMatrix(S);
        freeMatrix(L);
    }

    freeMatrix(M);
}