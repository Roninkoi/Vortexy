#include "phys.h"

void p_physInit(struct Phys* p)
{
	p->r = nvec3();
	p->v = nvec3();
	p->a = nvec3();
	p->f = nvec3();
	
	p->p = 0.0f;

	p->m = 1.0f;
}
