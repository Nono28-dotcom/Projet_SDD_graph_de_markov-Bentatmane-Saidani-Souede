#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse.h"
#include "utils.h"
#include "markov.h"

int main(void) {
    char fichier[256];
    printf("Choisissez parmi cette liste un fichier graphe et copiez-collez son nom :\n");
    printf("../Data/exemple1.txt\n../Data/exemple1_chatGPT_fixed.txt\n../Data/exemple2.txt\n../Data/exemple3.txt\n../Data/exemple4_2check.txt\n../Data/exemple_hasse1.txt\n../Data/exemple_scc1.txt\n../Data/exemple_valid_step3.txt\n");
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
        printf("Met son contenu dans https://www.mermaidchart.com/\n");
    } else {
        fprintf(stderr, "Erreur lors de la génération du fichier Mermaid.\n");
    }

    return 0;
}