#include "algebra.h"

// https://en.wikipedia.org/wiki/Gaussian_elimination#Pseudocode
mat GaussElim(mat *m)
{
	mat r = matCopy(m);

	int pr = 1;
	int pc = 1;

	while(pr < m->r && pc < m->c) {
	}

	return r;
}
