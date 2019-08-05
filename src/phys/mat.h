#ifndef MAT_H
#define MAT_H

#include "vec.h"

// general matrix
typedef struct {
	float **m;
	int r; // rows
	int c; // columns
} mat;

// 4 x 4 square matrix type
typedef struct {
	float m[4][4];
} mat4;

mat4 p_mat4(float s);

mat4 p_imat4(); // identity
mat4 p_nmat4(); // zero

vec4 p_mat4vec4(mat4 *m, vec4 *v); // mat4 x vec4
mat4 p_mat4mat4(mat4 *m0, mat4 *m1); // mat4 x mat4

mat4 p_mat4Translate(mat4 *m, vec4 v); // translate matrix by vec

mat4 p_mat4RotX(float a); // construct rotation matrix
mat4 p_mat4RotY(float a);

mat4 p_mat4RotateX(mat4 *m, float a); // rotate matrix by angle
mat4 p_mat4RotateY(mat4 *m, float a);

vec4 p_mat4GetRow(mat4 *m, int r);

mat4 p_mat4Transpose(mat4 *m); // transpose matrix

// construct perspective matrix
mat4 p_mat4Perspective(float fov, float aspect, float near, float far);

#endif
