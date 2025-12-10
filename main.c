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
    printf("    PROGRAMME D'ANALYSE DE GRAPHES DE MARKOV    \n\n");
    printf("Choisissez parmi cette liste un fichier graphe et copiez-collez son nom :\n");
    printf("../Data/exemple1.txt\n../Data/exemple1_chatGPT_fixed.txt\n../Data/exemple2.txt\n../Data/exemple3.txt\n../Data/exemple4_2check.txt\n../Data/exemple_hasse1.txt\n../Data/exemple_meteo.txt\n../Data/exemple_scc1.txt\n../Data/exemple_valid_step3.txt\n../Data/Matrice_Projet.txt\n");
    printf("Nom du fichier : ");
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


    liste_adjacence g = readGraph(input);
    printf("\nGraphe chargé avec succès (%d sommets).\n", g.taille);


    int choix;
    int continuer = 1;
    t_partition *partition = NULL;
    t_liens_array *liens = NULL;

    while (continuer) {
        printf("\n                                      \n");
        printf("         MENU PRINCIPAL\n");
        printf("                                        \n");
        printf("1. Vérifier si c'est un graphe de Markov\n");
        printf("2. Exporter le graphe au format Mermaid\n");
        printf("3. Algorithme de Tarjan (composantes fortement connexes)\n");
        printf("4. Diagramme de Hasse\n");
        printf("5. Analyser les caractéristiques du graphe\n");
        printf("6. Calculer la matrice stationnaire\n");
        printf("7. Distribution stationnaire par classes\n");
        printf("8. Tout exécuter\n");
        printf("0. Quitter\n");
        printf("                                        \n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {

            while (getchar() != '\n');
            printf("Choix invalide. Veuillez entrer un nombre.\n");
            continue;
        }

        printf("\n");

        switch (choix) {
            case 1:

                printf("    VÉRIFICATION GRAPHE DE MARKOV    \n");
                verif_graphe_Markov(g);
                break;

            case 2:

                printf("     EXPORT MERMAID     \n");
                if (export_to_mermaid(&g, "../sortie_effectuee.txt")) {
                    printf("Fichier Mermaid généré : ../sortie_effectuee.txt\n");
                    printf("Mettez son contenu dans https://www.mermaidchart.com/\n");
                } else {
                    fprintf(stderr, "Erreur lors de la génération du fichier Mermaid.\n");
                }
                break;

            case 3:

                printf("    ALGORITHME DE TARJAN    \n");
                if (partition != NULL) {
                    printf("Partition déjà calculée :\n");
                    afficher_partition(partition);
                } else {
                    partition = tarjan(g);
                    afficher_partition(partition);
                }
                break;

            case 4:

                printf("    DIAGRAMME DE HASSE    \n");
                if (partition == NULL) {
                    printf("Calcul de la partition (Tarjan) nécessaire...\n");
                    partition = tarjan(g);
                    afficher_partition(partition);
                    printf("\n");
                }

                if (liens != NULL) {
                    liberer_liens_array(liens);
                }

                liens = construire_hasse(g, partition);
                printf("Nombre de liens entre classes (avec redondances) : %d\n", liens->nb_liens);

                removeTransitiveLinks(liens);
                printf("Nombre de liens entre classes (sans redondances) : %d\n", liens->nb_liens);

                if (export_hasse_to_mermaid(partition, liens, "../hasse.txt")) {
                    printf("Diagramme de Hasse généré : ../hasse.txt\n");
                }
                break;

            case 5:

                printf("    CARACTÉRISTIQUES DU GRAPHE    \n");
                if (partition == NULL) {
                    printf("Calcul de la partition (Tarjan) nécessaire...\n");
                    partition = tarjan(g);
                    printf("\n");
                }
                if (liens == NULL) {
                    printf("Calcul du diagramme de Hasse nécessaire...\n");
                    liens = construire_hasse(g, partition);
                    removeTransitiveLinks(liens);
                    printf("\n");
                }

                analyser_caracteristiques(partition, liens);
                break;

            case 6:

                printf("    MATRICE STATIONNAIRE    \n");
                {
                    int n = g.taille;
                    float epsilon = 0.01f;

                    t_matrix M = createMatrixFromAdjacency(g);
                    printf("\nMatrice M :\n");
                    afficherMatrix(M);

                    t_matrix Mk  = createEmptyMatrix(n);
                    t_matrix Mk1 = createEmptyMatrix(n);

                    copyMatrix(Mk, M);
                    int k = 1;

                    while (1) {
                        multiplyMatrices(Mk, M, Mk1);
                        float d = diffMatrices(Mk, Mk1);
                        float d2 = ((int)(d * 100 + 0.5)) / 100.0f;

                        printf("\nDifférence M^%d -> M^%d = %.4f (arrondi = %.2f)\n", k, k+1, d, d2);

                        if (d2 <= epsilon) {
                            printf("Convergence atteinte : diff < %.2f\n", epsilon);
                            break;
                        }

                        copyMatrix(Mk, Mk1);
                        k++;

                        if (k > 50) {
                            printf("Pas de convergence après 50 itérations.\n");
                            break;
                        }
                    }

                    printf("\nMatrice finale (M^%d)\n", k);
                    afficherMatrix(Mk);

                    freeMatrix(M);
                    freeMatrix(Mk);
                    freeMatrix(Mk1);
                }
                break;

            case 7:

                printf("    DISTRIBUTION STATIONNAIRE PAR CLASSES    \n");
                if (partition == NULL) {
                    printf("Calcul de la partition (Tarjan) nécessaire...\n");
                    partition = tarjan(g);
                    printf("\n");
                }
                printStationaryForAllClasses(g, partition);
                break;

            case 8:

                printf("    EXÉCUTION COMPLÈTE    \n\n");

                printf("1. Vérification graphe de Markov\n");
                verif_graphe_Markov(g);

                printf("\n2. Export Mermaid\n");
                if (export_to_mermaid(&g, "../sortie_effectuee.txt")) {
                    printf("Fichier Mermaid généré : ../sortie_effectuee.txt\n");
                }

                printf("\n3. Algorithme de Tarjan\n");
                if (partition != NULL) {
                    liberer_partition(partition);
                }
                partition = tarjan(g);
                afficher_partition(partition);

                printf("\n4. Diagramme de Hasse\n");
                if (liens != NULL) {
                    liberer_liens_array(liens);
                }
                liens = construire_hasse(g, partition);
                printf("Nombre de liens (avec redondances) : %d\n", liens->nb_liens);
                removeTransitiveLinks(liens);
                printf("Nombre de liens (sans redondances) : %d\n", liens->nb_liens);
                export_hasse_to_mermaid(partition, liens, "../hasse.txt");
                printf("Diagramme de Hasse généré : ../hasse.txt\n");

                printf("\n5. Caractéristiques\n");
                analyser_caracteristiques(partition, liens);

                printf("\n6. Matrice stationnaire\n");
                {
                    int n = g.taille;
                    float epsilon = 0.01f;
                    t_matrix M = createMatrixFromAdjacency(g);
                    printf("\nMatrice M :\n");
                    afficherMatrix(M);

                    t_matrix Mk  = createEmptyMatrix(n);
                    t_matrix Mk1 = createEmptyMatrix(n);
                    copyMatrix(Mk, M);
                    int k = 1;

                    while (1) {
                        multiplyMatrices(Mk, M, Mk1);
                        float d = diffMatrices(Mk, Mk1);
                        float d2 = ((int)(d * 100 + 0.5)) / 100.0f;
                        printf("\nDiff M^%d -> M^%d = %.4f (arrondi = %.2f)\n", k, k+1, d, d2);

                        if (d2 <= epsilon) {
                            printf("Convergence atteinte\n");
                            break;
                        }
                        copyMatrix(Mk, Mk1);
                        k++;
                        if (k > 50) {
                            printf("Pas de convergence\n");
                            break;
                        }
                    }
                    printf("\nMatrice finale (M^%d)\n", k);
                    afficherMatrix(Mk);
                    freeMatrix(M);
                    freeMatrix(Mk);
                    freeMatrix(Mk1);
                }

                printf("\n7. Distribution stationnaire par classes\n");
                printStationaryForAllClasses(g, partition);
                break;


            case 9:
                printf("   DISTRIBUTIONS APRES 1, 2, 10 ET 50 PAS (X0 = 2)\n");

            {
                int etat_initial = 2;
                t_matrix M = createMatrixFromAdjacency(g);
                float *pi = malloc(g.taille * sizeof(float));

                int steps[4] = {1, 2, 10, 50};
                for (int i = 0; i < 4; i++) {
                    PiN(M, etat_initial, steps[i], pi);
                    printPiN(pi, steps[i], etat_initial);
                }

                free(pi);
                freeMatrix(M);
            }
            break;

            case 0:

                printf("Au revoir !\n");
                continuer = 0;
                break;

            default:
                printf("Choix invalide. Veuillez choisir un nombre entre 0 et 8.\n");
                break;
        }
    }



    if (partition != NULL) {
        liberer_partition(partition);
    }
    if (liens != NULL) {
        liberer_liens_array(liens);
    }

    return 0;
}


