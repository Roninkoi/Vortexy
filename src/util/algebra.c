#include "algebra.h"
#include "randomUtil.h"

void swapRows(mat *m, int r0, int r1)
{
	//if (r0 >= m->r || r1 >= m->r) return;
	
	for (int c = 0; c < m->c; ++c) {
		real old = m->m[r0][c];

		m->m[r0][c] = m->m[r1][c];
		m->m[r1][c] = old;
	}
}

int maxRow(mat *m, int r0, int c)
{
	//if (c >= m->c) return 0;
	
	real maxv = fabs(m->m[r0][c]);
	int maxr = r0;
	
	for (int r = r0 + 1; r < m->r; ++r) {
		real thisv = fabs(m->m[r][c]);
		
		if (thisv > maxv) {
			maxv = thisv;
			maxr = r;
		}
	}

	return maxr;
}

void GaussElim(mat *m)
{
	int pr = 0;
	int pc = 0;

	while(pr < m->r && pc < m->c) {
		int maxr = maxRow(m, pr, pc);

		if (m->m[maxr][pc] == 0.0f) {
			pc += 1;
		}
		else {
			swapRows(m, pr, maxr);

			for (int i = pr + 1; i < m->r; ++i) {
				real f = m->m[i][pc] / m->m[pr][pc];

				m->m[i][pc] = 0.0f;

				for (int j = pc + 1; j < m->c; ++j) {
					m->m[i][j] -= m->m[pr][j] * f;
				}
			}

			pr += 1;
			pc += 1;
		}
	}
}

void decompose(mat *a, mat *l, mat *u)
{
	*l = matGetL(a);
	*u = matGetSU(a);
}

int diagDom(mat *m)
{
	int n = min(m->r, m->c);
	
	for (int r = 0; r < n; ++r) {
		real sum = 0.0f;

		for (int c = 0; c < m->c; ++c) {
			if (r != c) {
				sum += m->m[r][c];
			}
		}

		if (sum > m->m[r][r]) return 0;
	}

	return 1;
}

void GaussSeidelSG(mat *a, mat *b, mat *g, int maxIt, real epsilon)
{
	int n = a->r;

	real delta = 0.0;

	int conv = 0;

	for (int k = 0; k < maxIt; ++k) {
		delta = 0.0;

		for (int i = 0; i < n; ++i) {
			real x = 1.0 / a->m[i][i];

			real s0 = 0.0;

			for (int j = a->rmin[i]; j <= i - 1; ++j) {
				s0 += a->m[i][j] * g->m[j][0];
			}

			real s1 = 0.0;

			for (int j = i + 1; j <= a->rmax[i]; ++j) {
				s1 += a->m[i][j] * g->m[j][0];
			}

			x *= (b->m[i][0] - s0 - s1);

			delta += g->m[i][0] - x;

			g->m[i][0] = x;
		}

		if (fabs(delta) < epsilon) { // has converged
			conv = 1;
			break;
		}
	}

	convergence &= conv;
}

mat GaussSeidelS(mat *a, mat *b, int maxIt, real epsilon)
{
	int n = a->r;

	mat r = matCopy(b);

	real delta = 0.0f;

	int conv = 0;

	for (int k = 0; k < maxIt; ++k) {
		delta = 0.0f;
		
		for (int i = 0; i < n; ++i) {
			real x = 1.0f / a->m[i][i];

			real s0 = 0.0f;

			for (int j = a->rmin[i]; j <= i - 1; ++j) {
				s0 += a->m[i][j] * r.m[j][0];
			}

			real s1 = 0.0f;
			
			for (int j = i + 1; j <= a->rmax[i]; ++j) {
				s1 += a->m[i][j] * r.m[j][0];
			}

			x *= (b->m[i][0] - s0 - s1);

			delta += r.m[i][0] - x;

			r.m[i][0] = x;
		}
		
		if (fabs(delta) < epsilon) { // has converged
			conv = 1;
			break;
		}
	}

	convergence &= conv;

	return r;
}

mat GaussSeidel(mat *a, mat *b, int maxIt, real epsilon)
{
	int n = a->r;

	mat r = matCopy(b);

	real delta = 0.0;

	for (int k = 0; k < maxIt; ++k) {
		delta = 0.0;
		
		for (int i = 0; i < n; ++i) {
			real x = 1.0 / a->m[i][i];

			real s0 = 0.0;

			for (int j = 0; j <= i - 1; ++j) {
				s0 += a->m[i][j] * r.m[j][0];
			}

			real s1 = 0.0;
			
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

