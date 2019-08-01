#include <util/randomUtil.h>
#include "phys/sys.h"

void p_sysInit(struct Sys *s)
{
	s->objs = NULL;
	s->objNum = 0;

	s->objs = malloc(sizeof(Obj));
	p_loadObj(&s->objs[0], "data/ico.obj");
	++s->objNum;
}

void p_sysTick(struct Sys *s)
{
	for (int i = 0; i < s->objs[0].mesh.colNum; i += 4) {
		float nc = randomFloat() * 2.0f * 3.14159f;
		s->objs[0].mesh.vertData[i+0] = s->objs[0].mesh.vertData[i+0] * 1.00f + 0.01f * sinf(nc);
		s->objs[0].mesh.vertData[i+1] = s->objs[0].mesh.vertData[i+1] * 1.00f + 0.01f * cosf(nc);
		s->objs[0].mesh.vertData[i+2] = s->objs[0].mesh.vertData[i+2] * 1.00f + 0.01f * sinf(nc) * cosf(nc);
	}
}
