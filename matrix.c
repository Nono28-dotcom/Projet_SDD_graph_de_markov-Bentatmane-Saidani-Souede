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

//La fonction subMatrix sert a extraire les sous matrices des classes en prenant en entré la matrice, la partition de tarjan et l'indice de la classe à extraire

t_matrix subMatrix(t_matrix M, t_partition *part, int c) {
    int k = part->classes[c].nb_sommets;
    //initialise la taille de la sous matrice, c'est a dire le nombre de sommets de la classe

    t_matrix S = createEmptyMatrix(k);
    int *indices = malloc(k * sizeof(int));
    //on crée une matrice vide de dimension k*k et un tableau indice qui servira à contenir tout les indices de cette sous matricre dans la matrice M originale


    for (int i = 0; i < k; i++) {
        indices[i] = part->classes[c].sommets[i].identifiant;

        if (indices[i] < 0 || indices[i] >= M.rows) {
            printf("ERREUR: sommet %d invalide dans classe %d\n",
                   part->classes[c].sommets[i].numero, c);
            exit(EXIT_FAILURE);
        }
    }
    //on rempli indices avec les identifiants des sommets dont on a besoin
    //on implémente également un crash contrôlé au cas ou le programe sort des bornes

    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            S.data[i][j] = M.data[ indices[i] ][ indices[j] ];
    //on copie la partie de M dont on a besoin dans S, grâce au tableau indice

    free(indices);
    return S;
    //on libère le tableau de stockage temporaire et on renvoie la sous matrice
}

//La fonction powerUntilLimit permet de calculer les puissances de la matrice jusqu'a qu'elle ateingne un état stationaire
//c'est a dire jusqu'a qu'elle soit stable
//elle prends en entré la sous matrice S, epsilon pour le critère d'arrêt et le nombre d'itération maximum

t_matrix powerUntilLimit(t_matrix S, float eps, int max_iter) {
    int n = S.rows;
    //on récupère la dimention n de la nouvelle sous matrice

    t_matrix Mk  = createEmptyMatrix(n);
    t_matrix Mk1 = createEmptyMatrix(n);
    //on crée les matrice S^k(Mk) et S^(k+1)(Mk1)

    copyMatrix(Mk, S);
    //on initialse M^1=S

    for (int k = 1; k <= max_iter; k++) {
        multiplyMatrices(Mk, S, Mk1);
        //on multiple les matrices pour chaque itération de boucle

        float d = diffMatrices(Mk, Mk1);
        float d2 = ((int)(d * 100 + 0.5)) / 100.0f;
        //on calcule la difference entre les deux matrices puis on arrondi le critère d'arrêt

        if (d2 < eps) {
            freeMatrix(Mk);
            return Mk1;
        }
        //on verifie si le critère d'arrêt deviens inferieur a epsilon, et on arrête le programme si c'est le cas en renvoyant la matrice stationaire



        copyMatrix(Mk, Mk1);
        //on prépare l'itération suivante
    }

    return Mk1;
    //retourne la dernière matrice a son état stationaire
}

// cette fonction printStationaryForAllClasses regroupe tout le processus demandé dans l'étape 2 de la partie 3
//à savoir l'extraction de la sous matrice des classes definies grâce a tarjan et le calcul de leur état stationnaire et de la distribution stationnaire
//Elle prends en entré la liste d'adjacence et la partition de tarjan

void printStationaryForAllClasses(liste_adjacence g, t_partition *part) {
    t_matrix M = createMatrixFromAdjacency(g);
    //On convertis le graph en matrice M


    printf("\nDistributions stationnaires par classe\n");

    for (int c = 0; c < part->nb_classes; c++) {
        //boucle sur chaque classe

        printf("\nClasse %d (taille %d)\n",
               c, part->classes[c].nb_sommets);

        t_matrix S = subMatrix(M, part, c);
        printf("\nSous-matrice S :\n \n");
        afficherMatrix(S);
        //on crée et affiche les sous matrices associés au classes

        t_matrix L = powerUntilLimit(S, 0.01f, 50);
        printf("\nMatrice limite S exposant oo :\n \n");
        afficherMatrix(L);
        //on utilise la foncion powerUntilLimit pour calculer l'état stationaires de ces denières puis on l'affiche

        int k = L.rows;
        float somme = 0;
        for (int j = 0; j < k; j++)
            somme += L.data[0][j];
        //on somme la première ligne (on aurait pu sommer une autre ligne car toutes les lignes sont égales lorsque la matrice est stationaire)

        printf("\nDistribution stationnaire : [");
        if (somme < 1e-6) {
            for (int j = 0; j < k; j++) printf("0 ");
            //on verifie si la somme est proche de zéro même si elle est censé valloir 1, pour eviter une division par zéro et un bug
        } else {
            for (int j = 0; j < k; j++)
                printf("%.3f ", L.data[0][j] / somme);
                //comme la somme est censé valloir 1, on affiche la distribution stationnaire de la matrice sur un seul vecteur probabilité
        }
        printf("]\n");


        freeMatrix(S);
        freeMatrix(L);
        //on libère l'espace alloué pour la sous matrice et la sous matrice stationnaire
    }

    freeMatrix(M);
    //on libère l'espace de la matrice M

}


// ============================================================
// Calcul de la distribution Pi(n) avec X0 = etat_initial (1 à N)
// ============================================================

void computePiN(t_matrix M, int etat_initial, int pas, float *result) {
    int n = M.rows;

    // Vecteurs temporaires
    float *pi  = calloc(n, sizeof(float));
    float *tmp = calloc(n, sizeof(float));

    // π(0) = e_(etat_initial)
    pi[etat_initial - 1] = 1.0f;

    // Calcul π(k+1) = π(k) * M
    for (int k = 0; k < pas; k++) {
        for (int j = 0; j < n; j++) tmp[j] = 0.0f;

        for (int j = 0; j < n; j++)
            for (int i = 0; i < n; i++)
                tmp[j] += pi[i] * M.data[i][j];

        for (int j = 0; j < n; j++) pi[j] = tmp[j];
    }

    // Copie dans result[]
    for (int j = 0; j < n; j++) result[j] = pi[j];

    free(pi);
    free(tmp);
}

// ============================================================
// Affichage formaté de π(n)
// ============================================================

void printPiN(float *pi, int n, int etat_initial) {
    printf("\nPI(%d) avec X0 = %d : [ ", n, etat_initial);
    for (int i = 0; i < 27; i++)
        printf("%.6f ", pi[i]);
    printf("]\n");
}
