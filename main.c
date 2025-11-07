#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse.h"
#include "utils.h"
#include "markov.h"

int main(void) {
    const char *input = "../Data/exemple1.txt";
    const char *mermaid_file = "../exemple_step3_mermaid.txt";

    liste_adjacence g = readGraph(input);

    if (export_to_mermaid(&g, mermaid_file)) {
        printf("Fichier Mermaid généré : %s\n", mermaid_file);
        printf("Met son contenu dans https://www.mermaidchart.com/\n");
    } else {
        fprintf(stderr, "Erreur lors de la génération du fichier Mermaid.\n");
    }

    return 0;
}