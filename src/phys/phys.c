#include "phys.h"

void p_physInit(struct Phys* p)
{
	p->r = p_nvec4();
	p->v = p_nvec4();
	p->a = p_nvec4();
	p->f = p_nvec4();
	
	p->p = 0.0f;

	p->m = 1.0f;
}
