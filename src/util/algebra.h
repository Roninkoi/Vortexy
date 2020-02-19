#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "mat.h"
#include "util.h"

// row reduction
void GaussElim(mat *m);

// GS method solve
mat GaussSeidel(mat *a, mat *b);

// decompose matrix into lower and strictly upper: A = L* + U
void decompose(mat *a, mat *b, mat *c);

void relax(mat *m, mat *m0);

// determine whether a matrix is diagonally dominant
int diagDom(mat *m);

// swap two rows of a matrix
void swapRows(mat *m, int r0, int r1);

// returns row index with abs maximum value
int maxRow(mat *m, int r0, int c);

#endif