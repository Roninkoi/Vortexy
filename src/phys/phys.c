#include "phys.h"

void p_physInit(struct Phys* p)
{
	p->r = nvec4();
	p->v = nvec4();
	p->a = nvec4();
	p->f = nvec4();
	
	p->p = 0.0f;

	p->m = 1.0f;
}
