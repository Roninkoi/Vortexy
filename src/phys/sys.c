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
			//s->objs[i].faces[j].mFlux = vec3Copy(&s->objs[i].faces[j].normal);
			//vec3Mul(&s->objs[i].faces[j].mFlux, s->objs[i].faces[j].initial);

			s->objs[i].faces[j].mFlux = vec3Copy(&s->objs[i].faces[j].pGrad);
			vec3Mul(&s->objs[i].faces[j].mFlux, 1.1f);
		}
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].phys.p = randomFloat();
		}
	}
}

void computeGrad(struct Sys *s) // not physical
{
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			for (int k = 0; k < 4; ++k) {
				struct Volume *vj = &s->objs[i].volumes[j];
				struct Face *fk = s->objs[i].volumes[j].faces[k];
				struct Volume *vk = p_volCmp(fk->thisVol[0], vj) ? fk->thisVol[1] : fk->thisVol[0];

				if (vk == NULL) // cannot compute gradient
					continue;

				vec3 r = vec3Copy(&vk->centroid);
				vec3Sub(&r, &vj->centroid);

				float rl = vec3Len(&r);

				float pDiff = vk->phys.p - vj->phys.p;

				fk->pGrad = vec3Copy(&r);//Vec3(pDiff / r.x, pDiff / r.y, pDiff / r.z);
				vec3Mul(&fk->pGrad, pDiff / rl);

				vec3 vDiff = vec3Copy(&vk->phys.v);
				vec3Sub(&vDiff, &vj->phys.v);

				fk->vGrad = vec3Copy(&r);
				vec3Mul(&fk->vGrad, vec3Len(&vDiff) / rl);
			}
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
	p_meshSetCol(&s->objs[0].mesh, 0.0f, 0.8f, 1.0f, 1.0f);

	computeGrad(s);

	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			s->objs[i].faces[j].mFlux = vec3Copy(&s->objs[i].faces[j].pGrad);
			vec3Mul(&s->objs[i].faces[j].mFlux, 1.1f);
		}
	}
}
