#include <string.h>

#include "util/randomUtil.h"
#include "sys.h"
#include "mat.h"

void p_sysInit(struct Sys *s)
{
	s->objs = NULL;
	s->objNum = 0;
}

void p_addObj(struct Sys *s, char *meshPath, char *fluidPath)
{
	const int oldNum = s->objNum;
	Obj *old = s->objs;

	++s->objNum;

	s->objs = malloc(sizeof(Obj) * s->objNum);

	for (int i = 0; i < oldNum; ++i) {
		if (s->objs == NULL)
			break;
		
		memcpy(&s->objs[i], &old[i], sizeof(Obj));
	}

	free(old);
	
	p_loadObj(&s->objs[oldNum], meshPath, fluidPath);
}

void p_sysStart(struct Sys *s)
{
	s->selected = 0;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			vec3 f = p_vec3Copy(&s->objs[i].faces[j].normal);
			p_vec3Mul(&f, s->objs[i].faces[j].initial);
			
			s->objs[i].faces[j].mFlux = f;
		}
	}
}

void p_sysTick(struct Sys *s)
{
	++s->ticks;
	p_meshSetCol(&s->objs[0].mesh, 0.0f, 0.8f, 1.0f, 1.0f);

	return;
	for (int i = 0; i < s->objs[0].mesh.colNum; i += 4) {
		float nc = randomFloat() * 2.0f * 3.14159f;
		s->objs[0].mesh.vertData[i+0] = s->objs[0].mesh.vertData[i+0] * 1.00f + 0.001f * sinf(nc);
		s->objs[0].mesh.vertData[i+1] = s->objs[0].mesh.vertData[i+1] * 1.00f + 0.001f * cosf(nc);
		s->objs[0].mesh.vertData[i+2] = s->objs[0].mesh.vertData[i+2] * 1.00f + 0.001f * sinf(nc) * cosf(nc);
	}
}
