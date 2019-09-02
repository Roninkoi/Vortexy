#include "mat.h"
#include "util/util.h"

mat Mat(float s, int r, int c)
{
	mat m;

	m.m = (float **) malloc(sizeof(float *) * r);

	for (int i = 0; i < r; ++i) {
		m.m[i] = (float *) calloc(c, sizeof(float));
	}

	for (int i = 0; i < min(r, c); ++i) {
		m.m[i][i] = s;
	}

	m.r = r;
	m.c = c;

	return m;
}

mat matAdd(mat *m0, mat *m1)
{
	mat r = Mat(0.0f, m0->r, m0->c);

	if (m0->r != m1->r || m0->c != m1->c)
		return r;

	for (int i = 0; i < m0->r; ++i) {
		for (int j = 0; j < m0->c; ++j) {
			r.m[i][j] = m0->m[i][j] + m1->m[i][j];
		}
	}

	return r;
}

mat matSub(mat *m0, mat *m1)
{
	mat r = Mat(0.0f, m0->r, m0->c);

	if (m0->r != m1->r || m0->c != m1->c)
		return r;

	for (int i = 0; i < m0->r; ++i) {
		for (int j = 0; j < m0->c; ++j) {
			r.m[i][j] = m0->m[i][j] - m1->m[i][j];
		}
	}

	return r;
}

mat matMul(mat *m0, mat *m1)
{
	mat r = Mat(0.0f, m0->r, m1->c);

	if (m0->c != m1->r)
		return r;

	for (int i = 0; i < m0->r; ++i) {
		for (int j = 0; j < m1->c; ++j) {
			for (int k = 0; k < m0->c; ++k) {
				r.m[i][j] += m0->m[i][k] * m1->m[k][j];
			}
		}
	}

	return r;
}

void matPrint(mat *m)
{
	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			printf("%.1f ", m->m[i][j]);
		}
		printf("\n");
	}
}

void matDestroy(mat *m)
{
	for (int i = 0; i < m->r; ++i) {
		free(m->m[i]);
	}
	free(m->m);
}

/*
  MAT4
*/

mat4 Mat4(float s)
{
	mat4 m;

	m.m[0][0] = s;
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = s;
	m.m[1][2] = 0.0f;
	m.m[1][3] = 0.0f;

	m.m[2][0] = 0.0f;
	m.m[2][1] = 0.0f;
	m.m[2][2] = s;
	m.m[2][3] = 0.0f;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = s;

	return m;
}

mat4 imat4() // identity
{
	return Mat4(1.0f);
}

mat4 nmat4() // zero
{
	return Mat4(0.0f);
}

mat4 mat4Add(mat4 *m0, mat4 *m1)
{
	mat4 r;
	
	r.m[0][0] = m0->m[0][0] + m1->m[0][0];
	r.m[0][1] = m0->m[0][1] + m1->m[0][1];
	r.m[0][2] = m0->m[0][2] + m1->m[0][2];
	r.m[0][3] = m0->m[0][3] + m1->m[0][3];
	
	r.m[1][0] = m0->m[1][0] + m1->m[1][0];
	r.m[1][1] = m0->m[1][1] + m1->m[1][1];
	r.m[1][2] = m0->m[1][2] + m1->m[1][2];
	r.m[1][3] = m0->m[1][3] + m1->m[1][3];
	
	r.m[2][0] = m0->m[2][0] + m1->m[2][0];
	r.m[2][1] = m0->m[2][1] + m1->m[2][1];
	r.m[2][2] = m0->m[2][2] + m1->m[2][2];
	r.m[2][3] = m0->m[2][3] + m1->m[2][3];
	
	r.m[3][0] = m0->m[3][0] + m1->m[3][0];
	r.m[3][1] = m0->m[3][1] + m1->m[3][1];
	r.m[3][2] = m0->m[3][2] + m1->m[3][2];
	r.m[3][3] = m0->m[3][3] + m1->m[3][3];

	return r;
}

mat4 mat4Sub(mat4 *m0, mat4 *m1)
{
	mat4 r;
	
	r.m[0][0] = m0->m[0][0] - m1->m[0][0];
	r.m[0][1] = m0->m[0][1] - m1->m[0][1];
	r.m[0][2] = m0->m[0][2] - m1->m[0][2];
	r.m[0][3] = m0->m[0][3] - m1->m[0][3];
	
	r.m[1][0] = m0->m[1][0] - m1->m[1][0];
	r.m[1][1] = m0->m[1][1] - m1->m[1][1];
	r.m[1][2] = m0->m[1][2] - m1->m[1][2];
	r.m[1][3] = m0->m[1][3] - m1->m[1][3];
	
	r.m[2][0] = m0->m[2][0] - m1->m[2][0];
	r.m[2][1] = m0->m[2][1] - m1->m[2][1];
	r.m[2][2] = m0->m[2][2] - m1->m[2][2];
	r.m[2][3] = m0->m[2][3] - m1->m[2][3];
	
	r.m[3][0] = m0->m[3][0] - m1->m[3][0];
	r.m[3][1] = m0->m[3][1] - m1->m[3][1];
	r.m[3][2] = m0->m[3][2] - m1->m[3][2];
	r.m[3][3] = m0->m[3][3] - m1->m[3][3];

	return r;
}

vec4 mat4vec4(mat4 *m, vec4 *v) // mat4 x vec4
{
	vec4 r = nvec4();

	r.x += m->m[0][0] * v->x;
	r.x += m->m[0][1] * v->y;
	r.x += m->m[0][2] * v->z;
	r.x += m->m[0][3] * v->w;

	r.y += m->m[1][0] * v->x;
	r.y += m->m[1][1] * v->y;
	r.y += m->m[1][2] * v->z;
	r.y += m->m[1][3] * v->w;

	r.z += m->m[2][0] * v->x;
	r.z += m->m[2][1] * v->y;
	r.z += m->m[2][2] * v->z;
	r.z += m->m[2][3] * v->w;

	r.w += m->m[3][0] * v->x;
	r.w += m->m[3][1] * v->y;
	r.w += m->m[3][2] * v->z;
	r.w += m->m[3][3] * v->w;

	return r;
}

mat4 mat4mat4(mat4 *m0, mat4 *m1) // mat4 x mat4
{
	mat4 r;

	// loops unrolled for performance on old compilers

	// row 0
	r.m[0][0] = m0->m[0][0] * m1->m[0][0]; // m0 row 0 dot m1 col 0
	r.m[0][0] += m0->m[0][1] * m1->m[1][0];
	r.m[0][0] += m0->m[0][2] * m1->m[2][0];
	r.m[0][0] += m0->m[0][3] * m1->m[3][0];

	r.m[0][1] = m0->m[0][0] * m1->m[0][1];
	r.m[0][1] += m0->m[0][1] * m1->m[1][1];
	r.m[0][1] += m0->m[0][2] * m1->m[2][1];
	r.m[0][1] += m0->m[0][3] * m1->m[3][1];

	r.m[0][2] = m0->m[0][0] * m1->m[0][2];
	r.m[0][2] += m0->m[0][1] * m1->m[1][2];
	r.m[0][2] += m0->m[0][2] * m1->m[2][2];
	r.m[0][2] += m0->m[0][3] * m1->m[3][2];

	r.m[0][3] = m0->m[0][0] * m1->m[0][3];
	r.m[0][3] += m0->m[0][1] * m1->m[1][3];
	r.m[0][3] += m0->m[0][2] * m1->m[2][3];
	r.m[0][3] += m0->m[0][3] * m1->m[3][3];

	// row 1
	r.m[1][0] = m0->m[1][0] * m1->m[0][0];
	r.m[1][0] += m0->m[1][1] * m1->m[1][0];
	r.m[1][0] += m0->m[1][2] * m1->m[2][0];
	r.m[1][0] += m0->m[1][3] * m1->m[3][0];

	r.m[1][1] = m0->m[1][0] * m1->m[0][1];
	r.m[1][1] += m0->m[1][1] * m1->m[1][1];
	r.m[1][1] += m0->m[1][2] * m1->m[2][1];
	r.m[1][1] += m0->m[1][3] * m1->m[3][1];

	r.m[1][2] = m0->m[1][0] * m1->m[0][2];
	r.m[1][2] += m0->m[1][1] * m1->m[1][2];
	r.m[1][2] += m0->m[1][2] * m1->m[2][2];
	r.m[1][2] += m0->m[1][3] * m1->m[3][2];

	r.m[1][3] = m0->m[1][0] * m1->m[0][3];
	r.m[1][3] += m0->m[1][1] * m1->m[1][3];
	r.m[1][3] += m0->m[1][2] * m1->m[2][3];
	r.m[1][3] += m0->m[1][3] * m1->m[3][3];

	// row 2
	r.m[2][0] = m0->m[2][0] * m1->m[0][0];
	r.m[2][0] += m0->m[2][1] * m1->m[1][0];
	r.m[2][0] += m0->m[2][2] * m1->m[2][0];
	r.m[2][0] += m0->m[2][3] * m1->m[3][0];

	r.m[2][1] = m0->m[2][0] * m1->m[0][1];
	r.m[2][1] += m0->m[2][1] * m1->m[1][1];
	r.m[2][1] += m0->m[2][2] * m1->m[2][1];
	r.m[2][1] += m0->m[2][3] * m1->m[3][1];

	r.m[2][2] = m0->m[2][0] * m1->m[0][2];
	r.m[2][2] += m0->m[2][1] * m1->m[1][2];
	r.m[2][2] += m0->m[2][2] * m1->m[2][2];
	r.m[2][2] += m0->m[2][3] * m1->m[3][2];

	r.m[2][3] = m0->m[2][0] * m1->m[0][3];
	r.m[2][3] += m0->m[2][1] * m1->m[1][3];
	r.m[2][3] += m0->m[2][2] * m1->m[2][3];
	r.m[2][3] += m0->m[2][3] * m1->m[3][3];

	// row 3
	r.m[3][0] = m0->m[3][0] * m1->m[0][0];
	r.m[3][0] += m0->m[3][1] * m1->m[1][0];
	r.m[3][0] += m0->m[3][2] * m1->m[2][0];
	r.m[3][0] += m0->m[3][3] * m1->m[3][0];

	r.m[3][1] = m0->m[3][0] * m1->m[0][1];
	r.m[3][1] += m0->m[3][1] * m1->m[1][1];
	r.m[3][1] += m0->m[3][2] * m1->m[2][1];
	r.m[3][1] += m0->m[3][3] * m1->m[3][1];

	r.m[3][2] = m0->m[3][0] * m1->m[0][2];
	r.m[3][2] += m0->m[3][1] * m1->m[1][2];
	r.m[3][2] += m0->m[3][2] * m1->m[2][2];
	r.m[3][2] += m0->m[3][3] * m1->m[3][2];

	r.m[3][3] = m0->m[3][0] * m1->m[0][3];
	r.m[3][3] += m0->m[3][1] * m1->m[1][3];
	r.m[3][3] += m0->m[3][2] * m1->m[2][3];
	r.m[3][3] += m0->m[3][3] * m1->m[3][3];

	return r;
}

mat4 mat4Translate(mat4 *m, vec4 v)
{
	mat4 r;

	mat4 tm = imat4();

	tm.m[0][3] = v.x;
	tm.m[1][3] = v.y;
	tm.m[2][3] = v.z;

	r = mat4mat4(m, &tm);

	return r;
}

vec4 mat4GetRow(mat4 *m, int r)
{
	vec4 v;

	v.x = m->m[r][0];
	v.y = m->m[r][1];
	v.z = m->m[r][2];
	v.w = m->m[r][3];

	return v;
}

vec4 mat4GetCol(mat4 *m, int c)
{
	vec4 v;

	v.x = m->m[0][c];
	v.y = m->m[1][c];
	v.z = m->m[2][c];
	v.w = m->m[3][c];

	return v;
}

void mat4SetRow(mat4 *m, vec4 *v, int r)
{
	m->m[r][0] = v->x;
	m->m[r][1] = v->y;
	m->m[r][2] = v->z;
	m->m[r][3] = v->w;
}

void mat4SetCol(mat4 *m, vec4 *v, int c)
{
	m->m[0][c] = v->x;
	m->m[1][c] = v->y;
	m->m[2][c] = v->z;
	m->m[3][c] = v->w;
}

mat4 mat4RotX(float a)
{
	mat4 m;

	m.m[0][0] = 1.0f;
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = cosf(a);
	m.m[1][2] = -sinf(a);
	m.m[1][3] = 0.0f;

	m.m[2][0] = 0.0f;
	m.m[2][1] = sinf(a);
	m.m[2][2] = cosf(a);
	m.m[2][3] = 0.0f;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	return m;
}

mat4 mat4RotY(float a)
{
	mat4 m;

	m.m[0][0] = cosf(a);
	m.m[0][1] = 0.0f;
	m.m[0][2] = sinf(a);
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = 1.0f;
	m.m[1][2] = 0.0f;
	m.m[1][3] = 0.0f;

	m.m[2][0] = -sinf(a);
	m.m[2][1] = 0.0f;
	m.m[2][2] = cosf(a);
	m.m[2][3] = 0.0f;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	return m;
}

mat4 mat4RotateX(mat4 *m, float a)
{
	mat4 r;

	mat4 rm;

	rm = mat4RotX(a);

	r = mat4mat4(m, &rm);

	return r;
}

mat4 mat4RotateY(mat4 *m, float a)
{
	mat4 r;

	mat4 rm;

	rm = mat4RotY(a);

	r = mat4mat4(m, &rm);

	return r;
}

mat4 mat4Transpose(mat4 *m)
{
	mat4 r;

	r.m[0][0] = m->m[0][0];
	r.m[0][1] = m->m[1][0];
	r.m[0][2] = m->m[2][0];
	r.m[0][3] = m->m[3][0];

	r.m[1][0] = m->m[0][1];
	r.m[1][1] = m->m[1][1];
	r.m[1][2] = m->m[2][1];
	r.m[1][3] = m->m[3][1];

	r.m[2][0] = m->m[0][2];
	r.m[2][1] = m->m[1][2];
	r.m[2][2] = m->m[2][2];
	r.m[2][3] = m->m[3][2];

	r.m[3][0] = m->m[0][3];
	r.m[3][1] = m->m[1][3];
	r.m[3][2] = m->m[2][3];
	r.m[3][3] = m->m[3][3];

	return r;
}

mat4 mat4Perspective(float fov, float aspect, float near, float far)
{
	mat4 m;

	m.m[0][0] = 1.0f / (aspect * tanf(0.5f * fov));
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = 1.0f / (tanf(0.5f * fov));
	m.m[1][2] = 0.0f;
	m.m[1][3] = 0.0f;

	m.m[2][0] = 0.0f;
	m.m[2][1] = 0.0f;
	m.m[2][2] = (-near - far) / (-near - far);
	m.m[2][3] = (2.0f * far * near) / (-near - far);

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 1.0f;
	m.m[3][3] = 0.0f;

	return m;
}