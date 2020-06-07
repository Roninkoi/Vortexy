#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "mat.h"
#include "util.h"

unsigned int msconvergence; // matrix solver converged?
unsigned int msiterations; // number of matrix solver iterations

// row reduction
void GaussElim(mat *m);

// GS method solve
void GaussSeidel(mat *a, mat *b, mat *g, int maxIt, real epsilon);

// solve matrix equation A x = b
void solve(mat *a, mat *b, mat *g, int maxIt, real epsilon);

// decompose matrix into lower and strictly upper: A = L* + U
void decompose(mat *a, mat *b, mat *c);

// determine whether a matrix is diagonally dominant
int diagDom(mat *m);

// swap two rows of a matrix
void swapRows(mat *m, int r0, int r1);

// returns row index with abs maximum value
int maxRow(mat *m, int r0, int c);

#endif
