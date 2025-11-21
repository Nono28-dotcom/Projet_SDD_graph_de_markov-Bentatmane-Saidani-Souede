//
// Created by nsoue on 21/11/2025.
//

#ifndef MATRIX_H
#define MATRIX_H


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

#endif //MATRIX_H
