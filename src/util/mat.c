#include "mat.h"
#include "util.h"
#include "randomUtil.h"

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

mat matCopy(mat *m)
{
	mat r = Mat(0.0f, m->r, m->c);

	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			r.m[i][j] = m->m[i][j];
		}
	}

	return r;
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

// product of diagonal elements
float matDiagProd(mat *m)
{
	float prod = 1.0f;

	if (m->r != m->c) return prod;

	for (int d = 0; d < m->r; ++d) {
		prod *= m->m[d][d];
	}

	return prod;
}

mat matTranspose(mat *m)
{
	mat r = Mat(0.0f, m->c, m->r);

	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			r.m[j][i] = m->m[i][j];
		}
	}

	return r;
}

mat matSubmatrix(mat *m, int i, int j)
{
	int nr = m->r - 1;
	int nc = m->c - 1;
	
	mat s = Mat(0.0f, nr, nc);

	int ri = 0;
	for (int r = 0; r < m->r; ++r) {
		for (int c = 0; c < m->c; ++c) {
			if (r != i && c != j) {
				s.m[ri / nr][ri % nc] = m->m[r][c]; // test
				++ri;
			}
		}
	}

	return s;
}

// determinant for 2x2 and 3x3
float matSarrDet(mat *m)
{
	if (m->r != m->c) return 0.0f;

	float sum = 0.0f;
	
	for (int c = 0; c < m->c; ++c) {
		float mulp = m->m[0][c];
		float muln = m->m[0][c];
		
		for (int d = 1; d < m->r; ++d) {
			mulp *= m->m[d][(c + d) % m->c];
			muln *= m->m[d][(c - d + m->c) % m->c]; // test
		}

		sum += mulp - muln;
	}

	return sum;
}

mat matRandom(float s, int r, int c)
{
	mat m = Mat(0.0f, r, c);

	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			m.m[i][j] = randomFloat() * s;
		}
	}

	return m;
}

void matPrint(mat *m)
{
	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			printf("%+.2f ", m->m[i][j]);
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


float matMinor(mat *m, int i, int j)
{
	mat s = matSubmatrix(m, i, j);

	float det = matDeterminant(&s);

	matDestroy(&s);
	
	return det;
}

float matCofactor(mat *m, int i, int j)
{
	float cf = signpowf(i + j) * matMinor(m, i, j);
	
	return cf;
}

// recursive determinant of nxn matrix
float matDeterminant(mat *m)
{
	if (m->r != m->c) return 0.0f;

	if (m->r <= 3) return matSarrDet(m);
	
	float sum = 0.0f;

	for (int c = 0; c < m->c; ++c) {
		float cf = matCofactor(m, 0, c);
		sum += m->m[0][c] * cf;
	}

	return sum;
}

mat matCofMat(mat *m)
{
	mat cm = Mat(0.0f, m->r, m->c);
	
	for (int r = 0; r < m->r; ++r) {
		for (int c = 0; c < m->c; ++c) {
			cm.m[r][c] = matCofactor(m, r, c);
		}
	}

	return cm;
}

mat matInverse(mat *m)
{
	mat r;

	float det = matDeterminant(m);

	if (det == 0.0f) return Mat(0.0f, m->r, m->c);

	mat cm = matCofMat(m);

	mat t = matTranspose(&cm);

	mat sm = Mat(1.0f / det, m->r, m->c);

	r = matMul(&t, &sm);

	matDestroy(&cm);
	matDestroy(&t);
	matDestroy(&sm);

	return r;
}

// get diagonal matrix
mat matGetD(mat *m)
{
	mat d = Mat(0.0f, m->r, m->c);
	
	if (m->r != m->c) return d;
		
	for (int rc = 0; rc < m->r; ++rc) {
		d.m[rc][rc] = m->m[rc][rc];
	}

	return d;
}

// get strictly upper triangular
mat matGetSU(mat *m)
{
	mat u = Mat(0.0f, m->r, m->c);
	
	if (m->r != m->c) return u;
		
	for (int r = 0; r < m->r; ++r) {
		for (int c = r + 1; c < m->c; ++c) {
			u.m[r][c] = m->m[r][c];
		}
	}

	return u;
}

// get lower triangular
mat matGetL(mat *m)
{
	mat l = Mat(0.0f, m->r, m->c);
	
	if (m->r != m->c) return l;
		
	for (int c = 0; c < m->c; ++c) {
		for (int r = c; r < m->r; ++r) {
			l.m[r][c] = m->m[r][c];
		}
	}

	return l;
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

void mat4Print(mat4 *m)
{
	printf("%+.3f %+.3f %+.3f %+.3f\n", m->m[0][0], m->m[0][1], m->m[0][2], m->m[0][3]);
	printf("%+.3f %+.3f %+.3f %+.3f\n", m->m[1][0], m->m[1][1], m->m[1][2], m->m[1][3]);
	printf("%+.3f %+.3f %+.3f %+.3f\n", m->m[2][0], m->m[2][1], m->m[2][2], m->m[2][3]);
	printf("%+.3f %+.3f %+.3f %+.3f\n", m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3]);
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

mat4 mat4Scale(mat4 *m, float s)
{
	mat4 r;
	
	mat4 sm = Mat4(s);

	r = mat4mat4(m, &sm);

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

mat mat4Submatrix(mat4 *m, int i, int j)
{
	mat s = Mat(0.0f, 3, 3);

	int ri = 0;
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			if (r != i && c != j) {
				s.m[ri / 3][ri % 3] = m->m[r][c]; // test
				++ri;
			}
		}
	}

	return s;
}

float mat4Determinant(mat4 *m)
{
	mat a = mat4Submatrix(m, 0, 0);
	mat b = mat4Submatrix(m, 0, 1);
	mat c = mat4Submatrix(m, 0, 2);
	mat d = mat4Submatrix(m, 0, 3);

	float da = matSarrDet(&a);
	float db = matSarrDet(&b);
	float dc = matSarrDet(&c);
	float dd = matSarrDet(&d);

	matDestroy(&a);
	matDestroy(&b);
	matDestroy(&c);
	matDestroy(&d);

	return m->m[0][0] * da - m->m[0][1] * db + m->m[0][2] * dc - m->m[0][3] * dd;
}

float mat4Minor(mat4 *m, int i, int j)
{
	mat s = mat4Submatrix(m, i, j);

	float det = matSarrDet(&s);

	matDestroy(&s);
	
	return det;
}

float mat4Cofactor(mat4 *m, int i, int j)
{
	return signpowf(i + j) * mat4Minor(m, i, j);
}

// cofactor 4-matrix
mat4 mat4CofMat(mat4 *m)
{
	mat4 cm;
	
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			cm.m[r][c] = mat4Cofactor(m, r, c);
		}
	}

	return cm;
}

// inverse 4-matrix m^(-1)
mat4 mat4Inverse(mat4 *m)
{
	mat4 r;
	
	float det = mat4Determinant(m);

	if (det == 0.0f) return nmat4();
	
	mat4 cm = mat4CofMat(m);

	r = mat4Transpose(&cm);
	r = mat4Scale(&r, 1.0f / det);

	return r;
}

