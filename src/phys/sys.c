#include <string.h>

#include "util/randomUtil.h"
#include "sys.h"
#include "util/mat.h"

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
			vec3 f = vec3Copy(&s->objs[i].faces[j].normal);
			vec3Mul(&f, s->objs[i].faces[j].initial);
			
			s->objs[i].faces[j].mFlux = f;
		}
	}
}

void jiggle(struct Sys *s)
{
	float intensity = 0.001f;
	
	for (int o = 0; o < s->objNum; ++o)
		for (int i = 0; i < s->objs[o].mesh.vertNum; i += 4) {
			float nc = randomFloat() * 2.0f * M_PI;
			s->objs[o].mesh.vertData[i+0] = s->objs[o].mesh.vertData[i+0] * 1.0f + intensity * sinf(nc);
			s->objs[o].mesh.vertData[i+1] = s->objs[o].mesh.vertData[i+1] * 1.0f + intensity * cosf(nc);
			s->objs[o].mesh.vertData[i+2] = s->objs[o].mesh.vertData[i+2] * 1.0f + intensity * sinf(nc) * cosf(nc);
		}
}

void p_sysTick(struct Sys *s)
{
	++s->ticks;
	p_meshSetCol(&s->objs[0].mesh, 0.0f, 0.8f, 1.0f, 1.0f);
}
