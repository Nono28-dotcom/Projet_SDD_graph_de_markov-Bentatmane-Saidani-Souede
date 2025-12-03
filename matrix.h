//
// Created by nsoue on 21/11/2025.
//

#ifndef MATRIX_H
#define MATRIX_H
#include "markov.h"
#include <math.h>

typedef struct {
    int rows;
    int cols;
    float **data;
} t_matrix;
//crée un type matrice, avec des lignes, collones, et des doubles pointeur

t_matrix createEmptyMatrix(int n);
void copyMatrix(t_matrix dest, t_matrix src);
void freeMatrix(t_matrix m);


t_matrix createMatrixFromAdjacency(liste_adjacence adj);

void multiplyMatrices(t_matrix A, t_matrix B, t_matrix result);

float diffMatrices(t_matrix A, t_matrix B);

void afficherMatrix(t_matrix m);

/**
* @brief Extracts a submatrix corresponding to a specific
component of a graph partition.
*
* @param matrix The original adjacency matrix of the graph.
* @param part The partition of the graph into strongly
connected components.
* @param compo_index The index of the component to extract.
* @return t_matrix The submatrix corresponding to the
specified component.
*/

t_matrix subMatrix(t_matrix matrix, t_partition *part, int compo_index);
t_matrix powerUntilLimit(t_matrix S, float eps, int max_iter);
void printStationaryForAllClasses(liste_adjacence g, t_partition *part);

void multiplyVectorMatrix(float *v, t_matrix M, float *res);
void distributionAfterN(t_matrix M, int n);

#endif //MATRIX_H
