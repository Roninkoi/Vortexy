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
			s->objs[i].faces[j].mFlux = vec3Copy(&s->objs[i].faces[j].normal);

			s->objs[i].faces[j].v = vec3Copy(&s->objs[i].faces[j].mFlux);
		}
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].p = (s->objs[i].volumes[j].centroid.x);
		}
	}
}

void p_sysTick(struct Sys *s)
{
	++s->ticks;
	p_meshSetCol(&s->objs[0].mesh, 0.0f, 0.8f, 1.0f, 1.0f);

	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			p_fp(&s->objs[i].faces[j]);
		}

		p_pGrad(&s->objs[i]);

		p_vGrad(&s->objs[i]);

		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			s->objs[i].faces[j].mFlux = vec3Copy(&s->objs->faces[j].pGrad);
		}
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].mFlux = vec3Copy(&s->objs->volumes[j].pGrad);
		}
	}
}
