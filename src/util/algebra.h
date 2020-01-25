#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "mat.h"

mat GaussElim(mat *m);

// GS method solve
mat GaussSeidel(mat *a, mat *b);

// decompose matrix into lower and strictly upper: A = L* + U
void decompose(mat *a, mat *b, mat *c);

#endif
