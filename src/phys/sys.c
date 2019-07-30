#include "phys/sys.h"

void p_sysInit(struct Sys *s)
{
	s->objs = NULL;
	s->objNum = 0;

	s->objs = malloc(sizeof(Obj));
	p_loadObj(&s->objs[0], "data/ico.obj");
	++s->objNum;
}
