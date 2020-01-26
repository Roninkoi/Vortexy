#ifndef MAT_H
#define MAT_H

#include "vec.h"

// general matrix
typedef struct {
	float **m;
	int r; // rows
	int c; // columns
} mat;

mat Mat(float s, int r, int c);

mat matCopy(mat *m);
float matDiagProd(mat *m);

mat matAdd(mat *m0, mat *m1);
mat matSub(mat *m0, mat *m1);
mat matMul(mat *m0, mat *m1);

float matDeterminant(mat *m);
mat matInverse(mat *m);
mat matTranspose(mat *m);

// diagonal
mat matGetD(mat *m);

// strictly upper
mat matGetSU(mat *m);

// lower
mat matGetL(mat *m);

mat matRandom(float s, int r, int c);

void matPrint(mat *m);

void matDestroy(mat *m);

// 4 x 4 square matrix type
typedef struct {
	float m[4][4];
} mat4;

mat4 Mat4(float s);

mat4 imat4(); // identity
mat4 nmat4(); // zero

vec4 mat4MulV(mat4 *m, vec4 *v); // mat4 x vec4
mat4 mat4Mul(mat4 *m0, mat4 *m1); // mat4 x mat4

mat4 mat4Add(mat4 *m0, mat4 *m1);
mat4 mat4Sub(mat4 *m0, mat4 *m1);

mat4 mat4Translate(mat4 *m, vec4 v); // translate matrix by vec

mat4 mat4Scale(mat4 *m, float s);

mat4 mat4RotX(float a); // construct rotation matrix
mat4 mat4RotY(float a);

mat4 mat4RotateX(mat4 *m, float a); // rotate matrix by angle
mat4 mat4RotateY(mat4 *m, float a);

vec4 mat4GetRow(mat4 *m, int r);
vec4 mat4GetCol(mat4 *m, int c);

void mat4SetRow(mat4 *m, vec4 *v, int r);
void mat4SetCol(mat4 *m, vec4 *v, int c);

mat4 mat4Transpose(mat4 *m); // transpose matrix

// construct perspective matrix
mat4 mat4Perspective(float fov, float aspect, float near, float far);

mat4 mat4Inverse(mat4 *m);
float mat4Determinant(mat4* m);

void mat4Print(mat4 *m);

#endif
