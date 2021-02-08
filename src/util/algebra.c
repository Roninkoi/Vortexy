#include "algebra.h"
#include "randomUtil.h"

//#define RELEPS

#define THREADNUM 16
#define MULTITHREADING_ENABLED 0

#if MULTITHREADING_ENABLED

#include <pthread.h>

#endif

void swapRows(mat *m, int r0, int r1)
{
	if (r0 >= m->r || r1 >= m->r) return;

	for (int c = 0; c < m->c; ++c) {
		real old = m->m[r0][c];

		m->m[r0][c] = m->m[r1][c];
		m->m[r1][c] = old;
	}
}

int maxRow(mat *m, int r0, int c)
{
	if (c >= m->c) return 0;

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

	while (pr < m->r && pc < m->c) {
		int maxr = maxRow(m, pr, pc);

		if (m->m[maxr][pc] == 0.0f) {
			pc += 1;
		} else {
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

#if MULTITHREADING_ENABLED

struct mt_args {
	mat *a;
	mat *b;
	mat *g;
	mat *gi;
	real epsilon;
	int start;
	int end;
	real *delta;
};

void *jcmt(void *args) // jacobi method
{
	struct mt_args *gargs = args;

	*gargs->delta = 0.0;

	for (int i = gargs->start; i < gargs->end; ++i) {
		real x = 1.0 / gargs->a->m[i][i];

		real s0 = 0.0;

		for (int j = gargs->a->rmin[i]; j <= gargs->a->rmax[i]; ++j) {
			if (j == i)
				continue;

			s0 += gargs->a->m[i][j] * gargs->gi->m[j][0];
		}

		x *= (gargs->b->m[i][0] - s0);

		if (x != x) {
			printf("NaN\n");
			continue;
		}

		real d = fabs(gargs->gi->m[i][0] - x);
#ifdef RELEPS
		d /= fabs(gargs->gi->m[i][0]);
#endif

		if (d > gargs->epsilon)
			*gargs->delta = d;

		gargs->g->m[i][0] = x;
	}
}

//#define TN8

void MTSolve(mat *a, mat *b, mat *g, int maxIt, real epsilon)
{
	int n = a->r;

	real delta = 0.0;

	int conv = 0;

	mat gi = matCopy(g);

	for (int k = 0; k < maxIt; ++k) {
		delta = 0.0;

#ifdef TN8
		pthread_t t0;
		pthread_t t1;
		pthread_t t2;
		pthread_t t3;
		pthread_t t4;
		pthread_t t5;
		pthread_t t6;
		pthread_t t7;

		real d0;
		real d1;
		real d2;
		real d3;
		real d4;
		real d5;
		real d6;
		real d7;

		matPaste(&gi, g);

		struct mt_args a0 = {a, b, g, &gi, epsilon, 0, n / 8, &d0};
		struct mt_args a1 = {a, b, g, &gi, epsilon, n / 8, 2 * n / 8, &d1};
		struct mt_args a2 = {a, b, g, &gi, epsilon, 2 * n / 8, 3 * n / 8, &d2};
		struct mt_args a3 = {a, b, g, &gi, epsilon, 3 * n / 8, 4 * n / 8, &d3};
		struct mt_args a4 = {a, b, g, &gi, epsilon, 4 * n / 8, 5 * n / 8, &d4};
		struct mt_args a5 = {a, b, g, &gi, epsilon, 5 * n / 8, 6 * n / 8, &d5};
		struct mt_args a6 = {a, b, g, &gi, epsilon, 6 * n / 8, 7 * n / 8, &d6};
		struct mt_args a7 = {a, b, g, &gi, epsilon, 7 * n / 8, n, &d7};

		pthread_create(&t0, NULL, jcmt, &a0);
		pthread_create(&t1, NULL, jcmt, &a1);
		pthread_create(&t2, NULL, jcmt, &a2);
		pthread_create(&t3, NULL, jcmt, &a3);
		pthread_create(&t4, NULL, jcmt, &a4);
		pthread_create(&t5, NULL, jcmt, &a5);
		pthread_create(&t6, NULL, jcmt, &a6);
		pthread_create(&t7, NULL, jcmt, &a7);

		pthread_join(t0, NULL);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);
		pthread_join(t4, NULL);
		pthread_join(t5, NULL);
		pthread_join(t6, NULL);
		pthread_join(t7, NULL);

		delta = max(d0, d1);
		delta = max(delta, d2);
		delta = max(delta, d3);
		delta = max(delta, d4);
		delta = max(delta, d5);
		delta = max(delta, d6);
		delta = max(delta, d7);
#else
		pthread_t t[THREADNUM];

		real d[THREADNUM];

		struct gsmt_args args[THREADNUM];

		for (int ti = 0; ti < THREADNUM; ++ti) {
			args[ti] = (struct mt_args) {a, b, g, &gi, epsilon,
					ti * n / THREADNUM,
					(ti + 1) * n / THREADNUM, &d[ti]};
			pthread_create(&t[ti], NULL, jcmt, &args[ti]);
		}

		for (int ti = 0; ti < THREADNUM; ++ti) {
			pthread_join(t[ti], NULL);
			delta = max(delta, d[ti]);
		}
#endif

		if (k > msiterations)
			msiterations = k;

		if (fabs(delta) < epsilon) { // has converged
			conv = 1;

			break;
		}
	}
	matDestroy(&gi);

	gsconvergence &= conv;
}
#endif

void GaussSeidel(mat *a, mat *b, mat *g, int maxIt, real epsilon)
{
	int n = a->r;

	real delta = 0.0;

	int conv = 0;

#if 0 // no guess
	matDestroy(g);
	*g = Matc(0.0, n);
#endif

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

			if (x != x) {
				printf("NaN a %f b %f s0 %f s1 %f \n", a->m[i][i], b->m[i][0], s0, s1);
				exit(0);
			}

			real d = fabs(g->m[i][0] - x);
#ifdef RELEPS
			d /= fabs(g->m[i][0]);
#endif

			if (d > epsilon)
				delta = d /* 0.5 + delta * 0.5*/;

			g->m[i][0] = x;
		}

		if (k > msiterations)
			msiterations = k;

		if (fabs(delta) < epsilon) { // has converged
			conv = 1;

			break;
		}
	}

	msconvergence &= conv;
}

void solve(mat *a, mat *b, mat *g, int maxIt, real epsilon)
{
#if MULTITHREADING_ENABLED
	MTSolve(a, b, g, maxIt, epsilon);
	return;
#endif

#if LAPACK_ENABLED
	LPS(a, b, g);
	return;
#endif

	GaussSeidel(a, b, g, maxIt, epsilon);
}
