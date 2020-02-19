#include "algebra.h"

void swapRows(mat *m, int r0, int r1)
{
	//if (r0 >= m->r || r1 >= m->r) return;
	
	for (int c = 0; c < m->c; ++c) {
		float old = m->m[r0][c];

		m->m[r0][c] = m->m[r1][c];
		m->m[r1][c] = old;
	}
}

int maxRow(mat *m, int r0, int c)
{
	//if (c >= m->c) return 0;
	
	float maxv = fabs(m->m[r0][c]);
	int maxr = r0;
	
	for (int r = r0 + 1; r < m->r; ++r) {
		float thisv = fabs(m->m[r][c]);
		
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
				float f = m->m[i][pc] / m->m[pr][pc];

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
		float sum = 0.0f;

		for (int c = 0; c < m->c; ++c) {
			if (r != c) {
				sum += m->m[r][c];
			}
		}

		if (sum > m->m[r][r]) return 0;
	}

	return 1;
}

mat GaussSeidel(mat *a, mat *b)
{
	int n = a->r;

	mat r = matCopy(b);

	/*if (!diagDom(a)) {
		printf("Not diag\n");
	}*/

	int maxIt = 10;
	float delta = 0.0f;
	float epsilon = 0.0001f;

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

			//if (!isnanf(x))
				r.m[i][0] = x;
		}
		
		if (fabs(delta) < epsilon) // has converged
			break;
	}

	return r;
}

