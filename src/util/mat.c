#include "mat.h"
#include "util.h"
#include "randomUtil.h"
#include <string.h>

mat Mat(real s, int r, int c)
{
	mat m;

	m.m = malloc(sizeof(real *) * r);

	for (int i = 0; i < r; ++i) {
		m.m[i] = calloc(c, sizeof(real));
	}

	for (int i = 0; i < min(r, c); ++i) {
		m.m[i][i] = s;
	}

	m.r = r;
	m.c = c;

	return m;
}

// column vector matrix
mat Matc(real s, int r)
{
	mat m;

	m.m = malloc(sizeof(real *) * r);

	for (int i = 0; i < r; ++i) {
		m.m[i] = calloc(1, sizeof(real));
	}

	for (int i = 0; i < r; ++i) {
		m.m[i][0] = s;
	}

	m.r = r;
	m.c = 1;

	return m;
}

real *matFlatten(mat *m)
{
	real *r = calloc(sizeof(real), m->r * m->c);
	//memcpy(r, &m->m, m->r * m->c);

	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			r[i * m->c + j] = m->m[i][j];
		}
	}

	return r;
}

mat matBuild(real *m, int r, int c)
{
	mat rm = Mat(0.0, r, c);

	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			rm.m[i][j] = m[i * c + j];
		}
	}

	return rm;
}

mat matVec3(vec3 *v)
{
	mat m = Mat(0.0, 3, 1);

	m.m[0][0] = v->x;
	m.m[1][0] = v->y;
	m.m[2][0] = v->z;

	return m;
}

mat matVec4(vec4 *v)
{
	mat m = Mat(0.0, 4, 1);

	m.m[0][0] = v->x;
	m.m[1][0] = v->y;
	m.m[2][0] = v->z;
	m.m[3][0] = v->w;

	return m;
}

vec3 vec3Mat(mat *m)
{
	if (m->r == 3) {
		return Vec3(m->m[0][0], m->m[1][0], m->m[2][0]);
	} else if (m->c == 3) {
		return Vec3(m->m[0][0], m->m[0][1], m->m[0][2]);
	}

	return Vec3(0.0, 0.0, 0.0);
}

mat matCopy(mat *m)
{
	mat r = Mat(0.0, m->r, m->c);

	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			r.m[i][j] = m->m[i][j];
		}
	}

	return r;
}

void matPaste(mat *r, mat *m)
{
	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			r->m[i][j] = m->m[i][j];
		}
	}
}

mat matAdd(mat *m0, mat *m1)
{
	mat r = Mat(0.0, m0->r, m0->c);

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
	mat r = Mat(0.0, m0->r, m0->c);

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
	mat r = Mat(0.0, m0->r, m1->c);

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

mat matDot(mat *m0, mat *m1)
{
	mat r = Mat(0.0, m0->c, 1);

	if (m0->c != m1->r)
		return r;

	for (int i = 0; i < m0->r; ++i) {
		for (int j = 0; j < m1->r; ++j) {
			r.m[i][0] += m0->m[i][j] * m1->m[j][0];
		}
	}

	return r;
}

// product of diagonal elements
real matDiagProd(mat *m)
{
	real prod = 1.0;

	if (m->r != m->c) return prod;

	for (int d = 0; d < m->r; ++d) {
		prod *= m->m[d][d];
	}

	return prod;
}

mat matTranspose(mat *m)
{
	mat r = Mat(0.0, m->c, m->r);

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

	mat s = Mat(0.0, nr, nc);

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
real matSarrDet(mat *m)
{
	if (m->r != m->c) return 0.0;

	real sum = 0.0;

	for (int c = 0; c < m->c; ++c) {
		real mulp = m->m[0][c];
		real muln = m->m[0][c];

		for (int d = 1; d < m->r; ++d) {
			mulp *= m->m[d][(c + d) % m->c];
			muln *= m->m[d][(c - d + m->c) % m->c]; // test
		}

		sum += mulp - muln;
	}

	return sum;
}

mat matRandom(real s, int r, int c)
{
	mat m = Mat(0.0, r, c);

	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			m.m[i][j] = randomFloat() * s;
		}
	}

	return m;
}

real matMax(mat *m)
{
	real max = 0.0;

	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			if (fabs(m->m[i][j]) > max) {
				max = fabs(m->m[i][j]);
			}
		}
	}

	return max;
}

void matPrint(mat *m)
{
	for (int i = 0; i < m->r; ++i) {
		for (int j = 0; j < m->c; ++j) {
			printf("%+.10f ", m->m[i][j]);
		}
		printf("\n");
	}
}

void matDestroy(mat *m)
{
	for (int i = 0; i < m->r; ++i) {
		if (m->m == NULL)
			break;

		free(m->m[i]);
	}
	free(m->m);
}

void matDestroyS(mat *m)
{
	for (int i = 0; i < m->r; ++i) {
		if (m->m == NULL)
			break;

		free(m->m[i]);
	}
	free(m->m);

	free(m->rmax);
	free(m->rmin);
}

real matMinor(mat *m, int i, int j)
{
	mat s = matSubmatrix(m, i, j);

	real det = matDeterminant(&s);

	matDestroy(&s);

	return det;
}

real matCofactor(mat *m, int i, int j)
{
	real cf = signpowf(i + j) * matMinor(m, i, j);

	return cf;
}

// recursive determinant of nxn matrix
real matDeterminant(mat *m)
{
	if (m->r != m->c) return 0.0;

	if (m->r <= 3) return matSarrDet(m);

	real sum = 0.0;

	for (int c = 0; c < m->c; ++c) {
		real cf = matCofactor(m, 0, c);
		sum += m->m[0][c] * cf;
	}

	return sum;
}

mat matCofMat(mat *m)
{
	mat cm = Mat(0.0, m->r, m->c);

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

	real det = matDeterminant(m);

	if (det == 0.0) return Mat(0.0, m->r, m->c);

	mat cm = matCofMat(m);

	mat t = matTranspose(&cm);

	mat sm = Mat(1.0 / det, m->r, m->c);

	r = matMul(&t, &sm);

	matDestroy(&cm);
	matDestroy(&t);
	matDestroy(&sm);

	return r;
}

// get diagonal matrix
mat matGetD(mat *m)
{
	mat d = Mat(0.0, m->r, m->c);

	if (m->r != m->c) return d;

	for (int rc = 0; rc < m->r; ++rc) {
		d.m[rc][rc] = m->m[rc][rc];
	}

	return d;
}

// get strictly upper triangular
mat matGetSU(mat *m)
{
	mat u = Mat(0.0, m->r, m->c);

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
	mat l = Mat(0.0, m->r, m->c);

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

mat4 Mat4(real s)
{
	mat4 m;

	m.m[0][0] = s;
	m.m[0][1] = 0.0;
	m.m[0][2] = 0.0;
	m.m[0][3] = 0.0;

	m.m[1][0] = 0.0;
	m.m[1][1] = s;
	m.m[1][2] = 0.0;
	m.m[1][3] = 0.0;

	m.m[2][0] = 0.0;
	m.m[2][1] = 0.0;
	m.m[2][2] = s;
	m.m[2][3] = 0.0;

	m.m[3][0] = 0.0;
	m.m[3][1] = 0.0;
	m.m[3][2] = 0.0;
	m.m[3][3] = s;

	return m;
}

mat4 imat4() // identity
{
	return Mat4(1.0);
}

mat4 nmat4() // zero
{
	return Mat4(0.0);
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

vec4 mat4MulV(mat4 *m, vec4 *v) // mat4 x vec4
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

mat4 mat4Mul(mat4 *m0, mat4 *m1) // mat4 x mat4
{
	mat4 r;

	// loops unrolled

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

	r = mat4Mul(m, &tm);

	return r;
}

mat4 mat4Scale(mat4 *m, real s)
{
	mat4 r;

	mat4 sm = Mat4(s);

	r = mat4Mul(m, &sm);

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

mat4 mat4RotX(real a)
{
	mat4 m;

	m.m[0][0] = 1.0;
	m.m[0][1] = 0.0;
	m.m[0][2] = 0.0;
	m.m[0][3] = 0.0;

	m.m[1][0] = 0.0;
	m.m[1][1] = cos(a);
	m.m[1][2] = -sin(a);
	m.m[1][3] = 0.0;

	m.m[2][0] = 0.0;
	m.m[2][1] = sin(a);
	m.m[2][2] = cos(a);
	m.m[2][3] = 0.0;

	m.m[3][0] = 0.0;
	m.m[3][1] = 0.0;
	m.m[3][2] = 0.0;
	m.m[3][3] = 1.0;

	return m;
}

mat4 mat4RotY(real a)
{
	mat4 m;

	m.m[0][0] = cos(a);
	m.m[0][1] = 0.0;
	m.m[0][2] = sin(a);
	m.m[0][3] = 0.0;

	m.m[1][0] = 0.0;
	m.m[1][1] = 1.0;
	m.m[1][2] = 0.0;
	m.m[1][3] = 0.0;

	m.m[2][0] = -sin(a);
	m.m[2][1] = 0.0;
	m.m[2][2] = cos(a);
	m.m[2][3] = 0.0;

	m.m[3][0] = 0.0;
	m.m[3][1] = 0.0;
	m.m[3][2] = 0.0;
	m.m[3][3] = 1.0;

	return m;
}

mat4 mat4RotateX(mat4 *m, real a)
{
	mat4 r;

	mat4 rm;

	rm = mat4RotX(a);

	r = mat4Mul(m, &rm);

	return r;
}

mat4 mat4RotateY(mat4 *m, real a)
{
	mat4 r;

	mat4 rm;

	rm = mat4RotY(a);

	r = mat4Mul(m, &rm);

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

mat4 mat4Perspective(real fov, real aspect, real near, real far)
{
	mat4 m;

	m.m[0][0] = 1.0 / (aspect * tan(0.5 * fov));
	m.m[0][1] = 0.0;
	m.m[0][2] = 0.0;
	m.m[0][3] = 0.0;

	m.m[1][0] = 0.0;
	m.m[1][1] = 1.0 / (tan(0.5 * fov));
	m.m[1][2] = 0.0;
	m.m[1][3] = 0.0;

	m.m[2][0] = 0.0;
	m.m[2][1] = 0.0;
	m.m[2][2] = (-near - far) / (-near - far);
	m.m[2][3] = (2.0 * far * near) / (-near - far);

	m.m[3][0] = 0.0;
	m.m[3][1] = 0.0;
	m.m[3][2] = 1.0;
	m.m[3][3] = 0.0;

	return m;
}

mat mat4Submatrix(mat4 *m, int i, int j)
{
	mat s = Mat(0.0, 3, 3);

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

real mat4Determinant(mat4 *m)
{
	mat a = mat4Submatrix(m, 0, 0);
	mat b = mat4Submatrix(m, 0, 1);
	mat c = mat4Submatrix(m, 0, 2);
	mat d = mat4Submatrix(m, 0, 3);

	real da = matSarrDet(&a);
	real db = matSarrDet(&b);
	real dc = matSarrDet(&c);
	real dd = matSarrDet(&d);

	matDestroy(&a);
	matDestroy(&b);
	matDestroy(&c);
	matDestroy(&d);

	return m->m[0][0] * da - m->m[0][1] * db + m->m[0][2] * dc - m->m[0][3] * dd;
}

real mat4Minor(mat4 *m, int i, int j)
{
	mat s = mat4Submatrix(m, i, j);

	real det = matSarrDet(&s);

	matDestroy(&s);

	return det;
}

real mat4Cofactor(mat4 *m, int i, int j)
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

	real det = mat4Determinant(m);

	if (det == 0.0) return nmat4();

	mat4 cm = mat4CofMat(m);

	r = mat4Transpose(&cm);
	r = mat4Scale(&r, 1.0 / det);

	return r;
}

/*
 * MAT3
 */

mat3 Mat3(real s)
{
	mat3 m;

	m.m[0][0] = s;
	m.m[0][1] = 0.0;
	m.m[0][2] = 0.0;

	m.m[1][0] = 0.0;
	m.m[1][1] = s;
	m.m[1][2] = 0.0;

	m.m[2][0] = 0.0;
	m.m[2][1] = 0.0;
	m.m[2][2] = s;

	return m;
}

mat3 imat3() // identity
{
	return Mat3(1.0);
}

mat3 nmat3() // zero
{
	return Mat3(0.0);
}

mat3 mat3Add(mat3 *m0, mat3 *m1)
{
	mat3 r;

	r.m[0][0] = m0->m[0][0] + m1->m[0][0];
	r.m[0][1] = m0->m[0][1] + m1->m[0][1];
	r.m[0][2] = m0->m[0][2] + m1->m[0][2];

	r.m[1][0] = m0->m[1][0] + m1->m[1][0];
	r.m[1][1] = m0->m[1][1] + m1->m[1][1];
	r.m[1][2] = m0->m[1][2] + m1->m[1][2];

	r.m[2][0] = m0->m[2][0] + m1->m[2][0];
	r.m[2][1] = m0->m[2][1] + m1->m[2][1];
	r.m[2][2] = m0->m[2][2] + m1->m[2][2];

	return r;
}

mat3 mat3Sub(mat3 *m0, mat3 *m1)
{
	mat3 r;

	r.m[0][0] = m0->m[0][0] - m1->m[0][0];
	r.m[0][1] = m0->m[0][1] - m1->m[0][1];
	r.m[0][2] = m0->m[0][2] - m1->m[0][2];

	r.m[1][0] = m0->m[1][0] - m1->m[1][0];
	r.m[1][1] = m0->m[1][1] - m1->m[1][1];
	r.m[1][2] = m0->m[1][2] - m1->m[1][2];

	r.m[2][0] = m0->m[2][0] - m1->m[2][0];
	r.m[2][1] = m0->m[2][1] - m1->m[2][1];
	r.m[2][2] = m0->m[2][2] - m1->m[2][2];

	return r;
}

vec3 mat3MulV(mat3 *m, vec3 *v) // mat3 x vec3
{
	vec3 r = nvec3();

	r.x += m->m[0][0] * v->x;
	r.x += m->m[0][1] * v->y;
	r.x += m->m[0][2] * v->z;

	r.y += m->m[1][0] * v->x;
	r.y += m->m[1][1] * v->y;
	r.y += m->m[1][2] * v->z;

	r.z += m->m[2][0] * v->x;
	r.z += m->m[2][1] * v->y;
	r.z += m->m[2][2] * v->z;

	return r;
}

mat3 mat3Mul(mat3 *m0, mat3 *m1) // mat3 x mat3
{
	mat3 r;

	// loops unrolled

	// row 0
	r.m[0][0] = m0->m[0][0] * m1->m[0][0]; // m0 row 0 dot m1 col 0
	r.m[0][0] += m0->m[0][1] * m1->m[1][0];
	r.m[0][0] += m0->m[0][2] * m1->m[2][0];

	r.m[0][1] = m0->m[0][0] * m1->m[0][1];
	r.m[0][1] += m0->m[0][1] * m1->m[1][1];
	r.m[0][1] += m0->m[0][2] * m1->m[2][1];

	r.m[0][2] = m0->m[0][0] * m1->m[0][2];
	r.m[0][2] += m0->m[0][1] * m1->m[1][2];
	r.m[0][2] += m0->m[0][2] * m1->m[2][2];

	// row 1
	r.m[1][0] = m0->m[1][0] * m1->m[0][0];
	r.m[1][0] += m0->m[1][1] * m1->m[1][0];
	r.m[1][0] += m0->m[1][2] * m1->m[2][0];

	r.m[1][1] = m0->m[1][0] * m1->m[0][1];
	r.m[1][1] += m0->m[1][1] * m1->m[1][1];
	r.m[1][1] += m0->m[1][2] * m1->m[2][1];

	r.m[1][2] = m0->m[1][0] * m1->m[0][2];
	r.m[1][2] += m0->m[1][1] * m1->m[1][2];
	r.m[1][2] += m0->m[1][2] * m1->m[2][2];

	// row 2
	r.m[2][0] = m0->m[2][0] * m1->m[0][0];
	r.m[2][0] += m0->m[2][1] * m1->m[1][0];
	r.m[2][0] += m0->m[2][2] * m1->m[2][0];

	r.m[2][1] = m0->m[2][0] * m1->m[0][1];
	r.m[2][1] += m0->m[2][1] * m1->m[1][1];
	r.m[2][1] += m0->m[2][2] * m1->m[2][1];

	r.m[2][2] = m0->m[2][0] * m1->m[0][2];
	r.m[2][2] += m0->m[2][1] * m1->m[1][2];
	r.m[2][2] += m0->m[2][2] * m1->m[2][2];

	return r;
}

mat3 mat3Scale(mat3 *m, real s)
{
	mat3 r;

	mat3 sm = Mat3(s);

	r = mat3Mul(m, &sm);

	return r;
}

mat3 mat3Transpose(mat3 *m)
{
	mat3 r;

	r.m[0][0] = m->m[0][0];
	r.m[0][1] = m->m[1][0];
	r.m[0][2] = m->m[2][0];

	r.m[1][0] = m->m[0][1];
	r.m[1][1] = m->m[1][1];
	r.m[1][2] = m->m[2][1];

	r.m[2][0] = m->m[0][2];
	r.m[2][1] = m->m[1][2];
	r.m[2][2] = m->m[2][2];

	return r;
}

mat3 mat3Copy(mat3 *m)
{
	mat3 r;

	r.m[0][0] = m->m[0][0];
	r.m[0][1] = m->m[0][1];
	r.m[0][2] = m->m[0][2];

	r.m[1][0] = m->m[1][0];
	r.m[1][1] = m->m[1][1];
	r.m[1][2] = m->m[1][2];

	r.m[2][0] = m->m[2][0];
	r.m[2][1] = m->m[2][1];
	r.m[2][2] = m->m[2][2];

	return r;
}

mat3 mat3MulVV(vec3 *v0, vec3 *v1) // vec3 x vec3
{
	mat3 r;

	r.m[0][0] = v0->x * v1->x;
	r.m[0][1] = v0->x * v1->y;
	r.m[0][2] = v0->x * v1->z;

	r.m[1][0] = v0->y * v1->x;
	r.m[1][1] = v0->y * v1->y;
	r.m[1][2] = v0->y * v1->z;

	r.m[2][0] = v0->z * v1->x;
	r.m[2][1] = v0->z * v1->y;
	r.m[2][2] = v0->z * v1->z;

	return r;
}

vec3 mat3DotV(mat3 *m, vec3 *v) // mat3 . vec3
{
	vec3 r = nvec3();

	r.x += m->m[0][0] * v->x;
	r.x += m->m[1][0] * v->y;
	r.x += m->m[2][0] * v->z;

	r.y += m->m[0][1] * v->x;
	r.y += m->m[1][1] * v->y;
	r.y += m->m[2][1] * v->z;

	r.z += m->m[0][2] * v->x;
	r.z += m->m[1][2] * v->y;
	r.z += m->m[2][2] * v->z;

	return r;
}