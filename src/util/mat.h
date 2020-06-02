#ifndef MAT_H
#define MAT_H

#include "vec.h"

// general matrix
typedef struct {
	real **m;
	int r; // rows
	int c; // columns

	int *rmin;
	int *rmax;
} mat;

mat Mat(real s, int r, int c);
mat Matc(real s, int r);

mat matCopy(mat *m);
void matPaste(mat *r, mat *m);
real matDiagProd(mat *m);

mat matAdd(mat *m0, mat *m1);
mat matSub(mat *m0, mat *m1);
mat matMul(mat *m0, mat *m1);
mat matDot(mat *m0, mat *m1);

real matDeterminant(mat *m);
mat matInverse(mat *m);
mat matTranspose(mat *m);

// diagonal
mat matGetD(mat *m);

// strictly upper
mat matGetSU(mat *m);

// lower
mat matGetL(mat *m);

mat matRandom(real s, int r, int c);

void matPrint(mat *m);

void matDestroy(mat *m);
void matDestroyS(mat *m);

mat matVec3(vec3 *v);
vec3 vec3Mat(mat *m);

mat matVec4(vec4 *v);

real matMax(mat *m);

// 4 x 4 square matrix type
typedef struct {
	real m[4][4];
} mat4;

mat4 Mat4(real s);

mat4 imat4(); // identity
mat4 nmat4(); // zero

vec4 mat4MulV(mat4 *m, vec4 *v); // mat4 x vec4
mat4 mat4Mul(mat4 *m0, mat4 *m1); // mat4 x mat4

mat4 mat4Add(mat4 *m0, mat4 *m1);
mat4 mat4Sub(mat4 *m0, mat4 *m1);

mat4 mat4Translate(mat4 *m, vec4 v); // translate matrix by vec

mat4 mat4Scale(mat4 *m, real s);

mat4 mat4RotX(real a); // construct rotation matrix
mat4 mat4RotY(real a);

mat4 mat4RotateX(mat4 *m, real a); // rotate matrix by angle
mat4 mat4RotateY(mat4 *m, real a);

vec4 mat4GetRow(mat4 *m, int r);
vec4 mat4GetCol(mat4 *m, int c);

void mat4SetRow(mat4 *m, vec4 *v, int r);
void mat4SetCol(mat4 *m, vec4 *v, int c);

mat4 mat4Transpose(mat4 *m); // transpose matrix

// construct perspective matrix
mat4 mat4Perspective(real fov, real aspect, real near, real far);

mat4 mat4Inverse(mat4 *m);
real mat4Determinant(mat4* m);

void mat4Print(mat4 *m);

// 3 x 3 square matrix type
typedef struct {
	real m[3][3];
} mat3;

mat3 Mat3(real s);

mat3 imat3(); // identity
mat3 nmat3(); // zero

mat3 mat3Copy(mat3 *m);

vec3 mat3MulV(mat3 *m, vec3 *v); // mat3 x vec3
mat3 mat3MulVV(vec3 *v0, vec3 *v1); // vec3 x vec3
mat3 mat3Mul(mat3 *m0, mat3 *m1); // mat3 x mat3
vec3 mat3DotV(mat3 *m, vec3 *v); // mat3 . vec3

mat3 mat3Add(mat3 *m0, mat3 *m1);
mat3 mat3Sub(mat3 *m0, mat3 *m1);

mat3 mat3Scale(mat3 *m, real s);

mat3 mat3Transpose(mat3 *m); // transpose matrix

#endif
