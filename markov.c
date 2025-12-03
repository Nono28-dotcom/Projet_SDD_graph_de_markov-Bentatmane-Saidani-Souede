//
// Created by nsoue on 01/11/2025.
//

#include "markov.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hasse.h"

//la fonction create cell crée le type cellule en prenant en entrée ses champs prob (contenant la probabilité pour aller d'un sommet à un autre) et arrivee (contenant le sommet vers lequel la probabilité est dirigé)
//on assimile cet cellule aux arêtes du graph

cell* creat_cell(float prob, int arrivee) {
  cell* new_cell = (cell*)malloc(sizeof(cell));
  //on alloue l'espace dynamiquement
  if (new_cell == NULL){
    return NULL;
  }
  //lignes de sécurité au cas où la mémoire n'est pas allouée
  new_cell->proba=prob;
  new_cell->sommet_arrivee=arrivee;
  new_cell->next=NULL;
  //la cellule n'a pas besoin de champ next, on fixe alors la valeur a NULL
  return new_cell;
};

//la fonction create_void_list crée une liste vide qui servira plus tard a contenir l'ensemble des cellules

liste create_void_list() {
  liste l;
  l.head = NULL;
    //initialise le champ head à NULL
  return l;
}

//La fonction ajouter_cellule prends en entrée la liste et les valeurs des champs prob et arrivee afin d'insérer une cellule dans la liste

void ajouter_cellule(liste *l, int arrivee, float prob) {
  cell *new_cell = creat_cell(prob, arrivee);
  //injecte les champs prob et arrivé dans la cellule
  new_cell->next = l->head;
  //insère en tête de liste (le next deviens l'ancien head)
  l->head = new_cell;
  //on ajoute la cellule dans la liste
};

//afficher les arêtes sortantes d'un sommet

void print_list(liste l, int sommet) {
  printf("Liste pour le sommet %d : [head @]", sommet);
  cell *tmp = l.head;
  //initialise le pointeur de parcours
  while (tmp != NULL) {
    printf(" -> (%d, %.2f)", tmp->sommet_arrivee, tmp->proba);
    tmp = tmp->next;
    //parcours la liste et s'arrete lorsque next=NULL (la liste est finie)
  }
  printf("\n");
};

//la fonction creer_liste_adjacence_vide permet de creer une liste dont chaque case contiendra une sous liste des aretes sortantes d'un sommet
//On peut assimiler cette liste a une matrice de probabilité du graph

liste_adjacence creer_liste_adjacence_vide(int taille) {
  liste_adjacence adj;
  adj.taille = taille;
  adj.tab = (liste*)malloc(taille * sizeof(liste));
  //initialise la taille du tableau et alloue de l'espace pour celui ci en fonction de la taille
  if (!adj.tab) {
    exit(EXIT_FAILURE);
  }
  //sécurité au cas ou l'allocation échoue
  for (int i = 0; i < taille; i++)
    adj.tab[i] = create_void_list();
  return adj;
  //boucle pour insérer les liste dans la liste d'adjacence
};


//cette fonction afficher_liste_adjacence parcours la liste d'adjacence et l'affiche

void afficher_liste_adjacence(liste_adjacence adj) {
  for (int i = 0; i < adj.taille; i++) {
    print_list(adj.tab[i], i + 1);
  }
};

//la fonction readGraph permet de lire un fichier contenant un graph et de le reconstruire sur C

liste_adjacence readGraph (const char* filename) {
  FILE *file = fopen(filename, "rt");
  //ouvre le fichier en mode lecture texte
  int nbvert, depart, arrivee;
  float proba;
  if (file == NULL) {
    perror("Could not open file for reading");
    exit(EXIT_FAILURE);
  }
  //verifie l'existence du fichier
  if (fscanf(file, "%d", &nbvert) != 1){
    perror("Could not read number of vertices");
    exit(EXIT_FAILURE);
  }
  //verifie l'ouverture
  liste_adjacence adj = creer_liste_adjacence_vide(nbvert);
  //crée le tableau d'adjacence vide
  while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3){
    ajouter_cellule(&adj.tab[depart - 1], arrivee - 1, proba);
  }
  //parcours du fichier et insertion dans la liste
  fclose(file);
  return adj;
};



//Cette fonction permet de vérifier si un graph est un graph de markov, donc de savoir si la somme de toutes les probas
//sortantes d'un sommet sont égales à 1

int verif_graphe_Markov(liste_adjacence adj) {
  int vrai = 1;
  //initialise la variable de "vérité" à vrai
  for (int i = 0; i < adj.taille; i++) {
    float cmp = 0.0;
    //initialise le compteur
    cell *tmp = adj.tab[i].head;
    //initialise un pointeur qui pointe sur le champ head de la sous liste
    while (tmp != NULL) {
      cmp += tmp->proba;
      tmp = tmp->next;
      //incrémente le compteur est passe à l'arête suivante
    }
    // parcours toute la liste d'adjacence, sous listes par sous listes, et somme la valeur des arrêtes avec un compteur
    if (cmp < 0.99 || cmp > 1.00) {
      printf("Le sommet %d n'est pas valide (P = %f)\n", i, cmp);
      vrai = 0;
    }
    //verifie si la somme est comprise entre 0.99 et 1, renvoie faux dans le cas contraire
  }
  if (vrai) {
    printf("Le graphe est un graphe de Markov.\n");
  } else {
    printf("Le graphe n'est pas un graphe de Markov.\n");
  }
  return vrai;
}


//la fonction export_to_mermaid prends un graph en entrée et l'exporte en format mermaid pour avoir une visualisation

int export_to_mermaid(const liste_adjacence *adj, const char *filename) {
  if (!adj || !filename)
    return 0;
  // verifie les pointeurs (liste d'adjacence et fichier)
  FILE *f = fopen(filename, "w");
  // ouvre le fichier en mode écriture
  if (!f) {
    perror("opening mermaid file"); return 0;
    // retourne une erreur au cas ou un problème a lieu lors de l'ouverture du fichier
  }

  fprintf(f, "---\n");
  fprintf(f, "config:\n");
  fprintf(f, "  layout: elk\n");
  fprintf(f, "  theme: neo\n");
  fprintf(f, "  look: neo\n");
  fprintf(f, "---\n");
  fprintf(f, "flowchart LR\n");
  //écrit le format pris en charge par mermaid dans le fichier (en tête mermaid)

  for (int i = 0; i < adj->taille; ++i) {
    char *id = getID(i + 1);
    //génére un identifiant mermaid unique
    fprintf(f, "%s((%d))\n", id, i + 1);
    //crée un sommet
  }
  fprintf(f, "\n");

  for (int i = 0; i < adj->taille; ++i) {
    cell *tmp = adj->tab[i].head;
    while (tmp != NULL) {
        fprintf(f, "%s -->|%.2f|%s\n", getID(i + 1), tmp->proba, getID(tmp->sommet_arrivee + 1));
      tmp = tmp->next;
    }
  }
  // parcours la liste d'adjacence et écrit dans le fichier la valeur des arrêtes
  fclose(f);
  return 1;
}

//cette fonction crée une pile vide avec une capacité donnée
t_pile* creer_pile(int capacite) {
    t_pile *p = (t_pile*)malloc(sizeof(t_pile));
    p->elements = (int*)malloc(capacite * sizeof(int));
    p->sommet = -1;   //lorsqu'elle est initialisée à -1, cela signifie qu'elle est vode
    p->capacite = capacite;
    return p;
}

//cette fonction vérifie si la pile est vide (si = -1)
int pile_vide(t_pile *p) {
    return p->sommet == -1;
}

//cette fonction ajoute un élément dans la pile si elle n'est pas pleine
void empiler(t_pile *p, int valeur) {
    if (p->sommet < p->capacite - 1) {
        p->sommet++;
        p->elements[p->sommet] = valeur;
    }
}

//cette fonction retire le dernier élément empilé dans la pile
int depiler(t_pile *p) {
    if (!pile_vide(p)) {
        int val = p->elements[p->sommet];
        p->sommet--;
        return val;
    }
    return -1;
}

//cette fonction libère la mémoire allouée pour la pile
void liberer_pile(t_pile *p) {
    free(p->elements);
    free(p);
}


//cette fonction crée une partition vide
t_partition* creer_partition() {
    t_partition *part = (t_partition*)malloc(sizeof(t_partition));
    part->classes = NULL;
    part->nb_classes = 0;
    part->capacite = 0;
    return part;
}

//cette fonction ajoute une nouvelle classe dans la partition et agrandi le tableau de la partition s'il ya trop de classes
void ajouter_classe(t_partition *part) {
    if (part->nb_classes >= part->capacite) {
        part->capacite = (part->capacite == 0) ? 2 : part->capacite * 2;
        part->classes = (t_classe*)realloc(part->classes, part->capacite * sizeof(t_classe));
    }

    t_classe *c = &part->classes[part->nb_classes];  //les classes sont initialisées et nomées
    sprintf(c->nom, "C%d", part->nb_classes + 1);
    c->sommets = NULL;
    c->nb_sommets = 0;
    c->capacite = 0;
    part->nb_classes++;
}

//cette fonction permet d'ajouter un sommet à une classe donnée
void ajouter_sommet_classe(t_classe *classe, t_tarjan_vertex sommet) {
    if (classe->nb_sommets >= classe->capacite) {
        classe->capacite = (classe->capacite == 0) ? 2 : classe->capacite * 2;
        classe->sommets = (t_tarjan_vertex*)realloc(classe->sommets,
                                                     classe->capacite * sizeof(t_tarjan_vertex));
    }
    classe->sommets[classe->nb_sommets] = sommet;
    classe->nb_sommets++;
}


//cette fonction permet d'afficher toutes les classes de la partition ainsi que leur sommet
void afficher_partition(t_partition *part) {
    for (int i = 0; i < part->nb_classes; i++) {    //on parcours toutes les classes de la partition
        printf("Composante %s: {", part->classes[i].nom);
        for (int j = 0; j < part->classes[i].nb_sommets; j++) {  //et on parcours tous les sommets de chaque classe
            printf("%d", part->classes[i].sommets[j].identifiant + 1);  //+1 pour afficher 1,2,3... au lieu de 0,1,2...
            if (j < part->classes[i].nb_sommets - 1) { //s'il reste encore des sommets, on met une virgule et on affiche les suivants
                printf(",");
            }
        }
        printf("}\n");
    }
}

//cette fonction permet de libérer la mémoire allouée pour la partition
void liberer_partition(t_partition *part) {
    for (int i = 0; i < part->nb_classes; i++) {
        free(part->classes[i].sommets);  //on libère d'abord les sommets
    }
    free(part->classes);  //puis on libère les classes
    free(part);           //et enfin toute la partition
}


//cette fonction permet d'initialiser le tableau des sommets de tarjan
t_tarjan_vertex* initialiser_tarjan_vertices(liste_adjacence adj) {
    t_tarjan_vertex *vertices = (t_tarjan_vertex*)malloc(adj.taille * sizeof(t_tarjan_vertex));

    for (int i = 0; i < adj.taille; i++) {
        vertices[i].identifiant = i;
        vertices[i].numero = -1;               //-1 si ça n'a pas été visité
        vertices[i].numero_accessible = -1;
        vertices[i].dans_pile = 0;             //0 si elle n'est pas dans la pile
    }

    return vertices;
}


//cette fonction permet de retourner le minimum entre deux variables de type int (entier)
int min(int a, int b) {
    return (a < b) ? a : b;
}


//cette fonction permet de faire un parcours en profondeur
void parcours(int sommet_id, liste_adjacence adj, t_tarjan_vertex *vertices,
              t_pile *pile, t_partition *part, int *compteur) {

    //les sommets sont numérotés
    vertices[sommet_id].numero = *compteur;
    vertices[sommet_id].numero_accessible = *compteur;
    (*compteur)++;

    //puis on les empile
    empiler(pile, sommet_id);
    vertices[sommet_id].dans_pile = 1;

    //puis on parcours les sommets suivants
    cell *tmp = adj.tab[sommet_id].head;
    while (tmp != NULL) {
        int succ_id = tmp->sommet_arrivee;

        if (vertices[succ_id].numero == -1) {   //s'il n'a pas été visité, alors on fait un parcours récursif
            parcours(succ_id, adj, vertices, pile, part, compteur);
            vertices[sommet_id].numero_accessible =
                min(vertices[sommet_id].numero_accessible,
                    vertices[succ_id].numero_accessible);
        }
        else if (vertices[succ_id].dans_pile == 1) {      //Le successeur a déjà été visité et il est toujours dans la pile on a donc trouvé un cycle
            vertices[sommet_id].numero_accessible =       //on met à jour numero_accessible avec le numéro du successeur
                min(vertices[sommet_id].numero_accessible,
                    vertices[succ_id].numero);
        }

        tmp = tmp->next;
    }

    //si c'est une racine de composante fortement connexe
    if (vertices[sommet_id].numero_accessible == vertices[sommet_id].numero) {
        ajouter_classe(part);
        t_classe *classe_courante = &part->classes[part->nb_classes - 1];

        //on dépile tous les sommets de la composante
        int w;
        do {
            w = depiler(pile);
            vertices[w].dans_pile = 0;
            ajouter_sommet_classe(classe_courante, vertices[w]);
        } while (w != sommet_id);
    }
}


//cette fonction permet de trouver toutes les composantes fortement connexes du graphe
t_partition* tarjan(liste_adjacence adj) {
    //on initialise une partition vide
    t_partition *part = creer_partition();
    //les sommets de tarjan sont initialisés
    t_tarjan_vertex *vertices = initialiser_tarjan_vertices(adj);
    //on crée une pile vide
    t_pile *pile = creer_pile(adj.taille);
    //ce compteur permet de numéroter les sommets
    int compteur = 0;
    //si un sommet n'a pas été visité ( == -1), alors on le parcours
    for (int i = 0; i < adj.taille; i++) {
        if (vertices[i].numero == -1) {
            parcours(i, adj, vertices, pile, part, &compteur);
        }
    }

    liberer_pile(pile);
    free(vertices);

    return part;
}


//cette fonction permet de créer un tableau vide de liens entre les classes
t_liens_array* creer_liens_array() {
    t_liens_array *array = (t_liens_array*)malloc(sizeof(t_liens_array));
    array->liens = NULL;
    array->nb_liens = 0;
    array->capacite = 0;
    return array;
}

//cette fonction permet de vérifier si un lien existe déjà
int lien_existe(t_liens_array *array, int depart, int arrivee) {
    for (int i = 0; i < array->nb_liens; i++) {
        if (array->liens[i].classe_depart == depart &&
            array->liens[i].classe_arrivee == arrivee) {
            return 1;
        }
    }
    return 0;
}

//cette fonction permet d'ajouter un lien entre deux classes
void ajouter_lien(t_liens_array *array, int depart, int arrivee) {
    if (lien_existe(array, depart, arrivee)) {
        return;   //cette vérification permet d'éviter les doublons
    }
    //s'il y a plus de liens que de capacité, alors on augmente la taille du tableau
    if (array->nb_liens >= array->capacite) {
        array->capacite = (array->capacite == 0) ? 2 : array->capacite * 2;
        array->liens = (t_lien*)realloc(array->liens, array->capacite * sizeof(t_lien));
    }
    //enfin, on ajoute le nouveau lien à la fin du tableau
    array->liens[array->nb_liens].classe_depart = depart;
    array->liens[array->nb_liens].classe_arrivee = arrivee;
    array->nb_liens++;
}

//cette fonction permet de créer un tableau qui associe chaque sommet à sa classe
int* creer_tableau_sommet_vers_classe(t_partition *part, int nb_sommets) {
    int *sommet_vers_classe = (int*)malloc(nb_sommets * sizeof(int));


    for (int i = 0; i < nb_sommets; i++) {
        sommet_vers_classe[i] = -1;  //initialisé à -1 car c'est un sommet dans classe
    }

    //pour chaque sommet, on note sa classe
    for (int i = 0; i < part->nb_classes; i++) {
        for (int j = 0; j < part->classes[i].nb_sommets; j++) {
            int sommet_id = part->classes[i].sommets[j].identifiant;
            sommet_vers_classe[sommet_id] = i; //i correspond au numéro de classe du sommet
        }
    }

    return sommet_vers_classe;
}


//cette fonction permet de faire le diagramme de Hasse, les liens entre les sommets
t_liens_array* construire_hasse(liste_adjacence adj, t_partition *part) {
    t_liens_array *liens = creer_liens_array();

    //on crée le tableau sommet vers classe
    int *sommet_vers_classe = creer_tableau_sommet_vers_classe(part, adj.taille);

    //pour chaque sommet
    for (int i = 0; i < adj.taille; i++) {
        int Ci = sommet_vers_classe[i];

        //ainsi que pour chaque successeur de i
        cell *tmp = adj.tab[i].head;
        while (tmp != NULL) {
            int j = tmp->sommet_arrivee;
            int Cj = sommet_vers_classe[j];

            //s'il y a un lien entre les deux, on l'ajoute
            if (Ci != Cj) {
                ajouter_lien(liens, Ci, Cj);
            }

            tmp = tmp->next;
        }
    }

    free(sommet_vers_classe);
    return liens;
}


//cette fonction permet de transformer le diagramme de Hasse au format Mermaid
int export_hasse_to_mermaid(t_partition *part, t_liens_array *liens, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("opening hasse file");
        return 0;
    }
    //cette en-tete est nécessaire pour pouvoir afficher le graphe sur le site mermaid
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "  layout: elk\n");
    fprintf(f, "  theme: neo\n");
    fprintf(f, "  look: neo\n");
    fprintf(f, "---\n");
    fprintf(f, "flowchart TD\n");

    //on déclare les classes avec leurs sommets
    for (int i = 0; i < part->nb_classes; i++) {
        fprintf(f, "%s[\"{", part->classes[i].nom);
        for (int j = 0; j < part->classes[i].nb_sommets; j++) {
            fprintf(f, "%d", part->classes[i].sommets[j].identifiant + 1);
            if (j < part->classes[i].nb_sommets - 1) {
                fprintf(f, ",");
            }
        }
        fprintf(f, "}\"]\n");
    }
    fprintf(f, "\n");

    //puis on déclare les liens entre les classes
    for (int i = 0; i < liens->nb_liens; i++) {
        fprintf(f, "%s --> %s\n",
                part->classes[liens->liens[i].classe_depart].nom,
                part->classes[liens->liens[i].classe_arrivee].nom);
    }

    fclose(f);
    return 1;
}

//cette fonction permet de libérer la mémoire d'un tableau de liens
void liberer_liens_array(t_liens_array *array) {
    free(array->liens);
    free(array);
}

//cette fonction permet d'analyser et d'afficher les caractéristiques du graphe de Markov
int analyser_caracteristiques(t_partition *part, t_liens_array *liens) {
    if (!part) return 0;
    int nb_classes = part->nb_classes;

    if (nb_classes == 0) {
        printf("Aucune classe trouvée.\n");
        return 0;
    }

    //on crée un tableau pour marquer les classes qui ont des sorties
    int *a_sortie = (int*)calloc(nb_classes, sizeof(int));
    if (!a_sortie) { perror("calloc"); return 0; }

    //on marque les classes avec des liens sortants (qui sont transitoires)
    if (liens) {
        for (int i = 0; i < liens->nb_liens; ++i) {
            int d = liens->liens[i].classe_depart;
            int a = liens->liens[i].classe_arrivee;
            if (d >= 0 && d < nb_classes && a >= 0 && a < nb_classes) {
                if (d != a) a_sortie[d] = 1;
            }
        }
    }
    //on affiche les caractéristiques des classes
    printf("Caractéristiques des classes :\n");
    for (int c = 0; c < nb_classes; ++c) {
        const char *type_cl = a_sortie[c] ? "transitoire" : "persistante";
        printf("Classe %s: %s (nb sommets = %d)\n", part->classes[c].nom, type_cl, part->classes[c].nb_sommets);
    }
    //et ici, on affiche celles des états (les sommets)
    printf("\nCaractéristiques des états (sommets) :\n");
    for (int c = 0; c < nb_classes; ++c) {
        int class_transitoire = a_sortie[c];
        for (int s = 0; s < part->classes[c].nb_sommets; ++s) {
            int vid = part->classes[c].sommets[s].identifiant;
            const char *etat_type;
            if (class_transitoire) {
                etat_type = "transitoire";
            } else {
                //on vérifie si c'est une classe persistante : absorbant si 1 seul sommet
                if (part->classes[c].nb_sommets == 1) etat_type = "absorbant";
                else etat_type = "persistant";
            }
            printf("État %d : %s (dans classe %s)\n", vid + 1, etat_type, part->classes[c].nom);
        }
    }
    //on vérifie si le graphe est irréductible, s'il n'y a qu'une seule classe
    int irreductible = (nb_classes == 1) ? 1 : 0;
    printf("\nGraphe irréductible ? %s\n", irreductible ? "OUI" : "NON");

    free(a_sortie);
    return irreductible;
}

