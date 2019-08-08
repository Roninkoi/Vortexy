#include <util/randomUtil.h>
#include "sys.h"

void p_sysInit(struct Sys *s)
{
	s->objs = NULL;
	s->objNum = 0;

	s->objs = malloc(sizeof(Obj));
	//p_loadObj(&s->objs[0], "data/pipe.obj");
	p_loadObj(&s->objs[0], "data/test.obj");
	++s->objNum;

	//p_meshSetCol(&s->objs[0].mesh, 1.0f, 0.5f, 0.0f, 1.0f);
}

void p_sysTick(struct Sys *s)
{
	++s->ticks;
	return;
	for (int i = 0; i < s->objs[0].mesh.colNum; i += 4) {
		float nc = randomFloat() * 2.0f * 3.14159f;
		s->objs[0].mesh.vertData[i+0] = s->objs[0].mesh.vertData[i+0] * 1.00f + 0.001f * sinf(nc);
		s->objs[0].mesh.vertData[i+1] = s->objs[0].mesh.vertData[i+1] * 1.00f + 0.001f * cosf(nc);
		s->objs[0].mesh.vertData[i+2] = s->objs[0].mesh.vertData[i+2] * 1.00f + 0.001f * sinf(nc) * cosf(nc);
	}
}
