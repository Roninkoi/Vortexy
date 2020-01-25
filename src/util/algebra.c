#include "algebra.h"

mat GaussElim(mat *m)
{
	mat r = matCopy(m);

	int pr = 1;
	int pc = 1;

	while(pr < m->r && pc < m->c) {
	}

	return r;
}

void decompose(mat *a, mat *l, mat *u)
{
	*l = matGetL(a);
	*u = matGetSU(a);
}

mat GaussSeidel(mat *a, mat *b)
{
	int n = a->r;

	mat r = matCopy(b);

	int maxIt = 20;
	float delta = 0.0f;
	float epsilon = 0.1f;

	for (int k = 0; k < maxIt; ++k) {
		delta = 0.0f;
		
		for (int i = 0; i < n; ++i) {
			float x = 1.0f / a->m[i][i];

			float s0 = 0.0f;

			for (int j = 0; j <= i - 1; ++j) {
				s0 += a->m[i][j] * r.m[j][0];
			}

			float s1 = 0.0f;
			
			for (int j = i + 1; j < n; ++j) {
				s1 += a->m[i][j] * r.m[j][0];
			}

			x *= (b->m[i][0] - s0 - s1);

			delta += r.m[i][0] - x;

			r.m[i][0] = x;
		}
		
		if (fabs(delta) < epsilon) // has converged
			break;
	}

	return r;
}

