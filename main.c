#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse.h"
#include "utils.h"
#include "markov.h"
#include "hasse.h"
#include "matrix.h"

int main(void) {
    char fichier[256];
    printf("Choisissez parmi cette liste un fichier graphe et copiez-collez son nom :\n");
    printf("../Data/exemple1.txt\n../Data/exemple1_chatGPT_fixed.txt\n../Data/exemple2.txt\n../Data/exemple3.txt\n../Data/exemple4_2check.txt\n../Data/exemple_hasse1.txt\n../Data/exemple_meteo.txt\n../Data/exemple_scc1.txt\n../Data/exemple_valid_step3.txt\n");
    printf("Nom du fichier :\n");
    if (fgets(fichier, sizeof(fichier), stdin) == NULL) {
        fprintf(stderr, "Lecture du nom de fichier impossible.\n");
        return 1;
    }

    fichier[strcspn(fichier, "\n")] = '\0';

    if (fichier[0] == '\0') {
        fprintf(stderr, "Aucun nom de fichier saisi.\n");
        return 1;
    }

    const char *input = fichier;
    const char *mermaid_file = "../sortie_effectuee.txt";

    liste_adjacence g = readGraph(input);

    verif_graphe_Markov(g);

    if (export_to_mermaid(&g, mermaid_file)) {
        printf("Fichier Mermaid généré : %s\n", mermaid_file);
        printf("Met son contenu dans https://www.mermaidchart.com/ et le graphe associé sera généré\n");
    } else {
        fprintf(stderr, "Erreur lors de la génération du fichier Mermaid.\n");
    }

    printf("\nAlgorithme de Tarjan :\n");
    t_partition *partition = tarjan(g);
    afficher_partition(partition);

    printf("\nDiagramme de Hasse :\n");
    t_liens_array *liens = construire_hasse(g, partition);
    printf("Nombre de liens entre classes (avec redondances) : %d\n", liens->nb_liens);


    removeTransitiveLinks(liens);
    printf("Nombre de liens entre classes (sans redondances) : %d\n", liens->nb_liens);

    if (export_hasse_to_mermaid(partition, liens, "../hasse.txt")) {
        printf("Diagramme de Hasse généré : ../hasse.txt\n");
    }

    analyser_caracteristiques(partition, liens);









    int n = g.taille;
    float epsilon = 0.01f;
    //on initialise la taille de la matrice (nombres d'états) et epsilon (critère d'arrêt lorsque la différence entre M^k et M^(k+1) < 0.01)


    t_matrix M = createMatrixFromAdjacency(g);
    printf("\nMatrice M :\n");
    afficherMatrix(M);
    //Convertit la liste d'adjacence du graph en matrice de probabilité


    t_matrix Mk  = createEmptyMatrix(n);
    t_matrix Mk1 = createEmptyMatrix(n);
    //Mk va contenir les matrice avec différents exposants k et Mk1 stock M^(k+1) temporairement

    copyMatrix(Mk, M);
    int k = 1;
    //On initialise (M^1=M)

    while (1) {
        multiplyMatrices(Mk, M, Mk1);
        //on effectue le produit matriciel et on stock le résultat dans Mk1

        float d = diffMatrices(Mk, Mk1);
        //on calcule la différence entre M^k et M^(k+1)

        float d2 = ((int)(d * 100 + 0.5)) / 100.0f;
        //on arrondi le critère d'arrêt pour que la matrice stationaire renvoyée a la fin soit la même dans que dans les consignes
        printf("\nDifférence M^%d -> M^%d = %.4f (arrondi = %.2f)\n", k, k+1, d, d2);
        //affiche la différence entre M^k et M^(k+1)

        if (d2 <= epsilon) {
            printf("Convergence atteinte : diff < %.2f\n", epsilon);
            break;
        }
        //verifie le critère d'arrêt et sort de la boucle si il est respecté (la différence est inferieure à epsilon)

        copyMatrix(Mk, Mk1);
        k++;
        //on incrémente et on continue la boucle jusqu'a en sortir

        if (k > 30) {
            printf("Pas de convergence\n");
            break;
        }
        //si la boucle parcours trop d'itération, on sort de cette dernière
    }

    printf("\nMatrice finale (M^%d)\n", k);
    afficherMatrix(Mk);
    freeMatrix(M);
    freeMatrix(Mk);
    freeMatrix(Mk1);
    //affiche la matrice finale, et libère les 3 matrices pour lesquelles on a alloué de l'espace


    printStationaryForAllClasses(g, partition);

    liberer_liens_array(liens);
    liberer_partition(partition);

    return 0;
}


