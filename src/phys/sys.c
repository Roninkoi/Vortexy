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
		p_decomposeSurfs(&s->objs[i]);

		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			s->objs[i].faces[j].mFlux = vec3Copy(&s->objs[i].faces[j].normal);

			s->objs[i].faces[j].v = vec3Copy(&s->objs[i].faces[j].mFlux);
		}
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].p = (s->objs[i].volumes[j].centroid.x) + 1.0f;
		}
	}
}

void jiggle(struct Sys *s)
{
	float intensity = 0.001f;

	for (int o = 0; o < s->objNum; ++o)
		for (int i = 0; i < s->objs[o].mesh.vertNum; i += 4) {
			float nc = randomFloat() * 2.0f * M_PI;
			s->objs[o].mesh.vertData[i + 0] = s->objs[o].mesh.vertData[i + 0] * 1.0f + intensity * sinf(nc);
			s->objs[o].mesh.vertData[i + 1] = s->objs[o].mesh.vertData[i + 1] * 1.0f + intensity * cosf(nc);
			s->objs[o].mesh.vertData[i + 2] = s->objs[o].mesh.vertData[i + 2] * 1.0f + intensity * sinf(nc) * cosf(nc);
		}
}

void p_sysTick(struct Sys *s)
{
	++s->ticks;
	p_meshSetCol(&s->objs[0].mesh, 0.0f, 0.08f, 0.10f, 0.10f);

	s->t += s->dt;

	for (int i = 0; i < s->objNum; ++i) {
		s->objs[i].dt = s->dt;

		p_computeFaceFs(&s->objs[i]);
		p_computeVolFs(&s->objs[i]);
		p_computeCoeffs(&s->objs[i]);

		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			p_faceP(&s->objs[i].faces[j]);
			p_faceV(&s->objs[i].faces[j]);
		}

		p_pGrad(&s->objs[i]);

		p_vGrad(&s->objs[i]);

		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			s->objs[i].faces[j].mFlux = vec3Copy(&s->objs->faces[j].pGrad);
		}
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].mFlux = vec3Copy(&s->objs->volumes[j].pGrad);

			s->objs[i].volumes[j].p = (s->objs[i].volumes[j].centroid.x) * (sinf(s->time)+0.5f) + (s->objs[i].volumes[j].centroid.y) * (cosf(s->time)+0.5f);
		}

		p_constructMats(&s->objs[i]);

		matPrint1(&s->objs[i].a);
		printf("\n");

		matDestroy(&s->objs[i].a);
		matDestroy(&s->objs[i].v);
		matDestroy(&s->objs[i].b);
	}
}
