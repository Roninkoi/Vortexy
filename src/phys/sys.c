#include <string.h>
#include <util/algebra.h>

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
<<<<<<< HEAD
		s->objs[i].f = Vec3(0.0f, 0.0f, 0.0f);
		s->objs[i].t = 0.0f;

		p_decomposeSurfs(&s->objs[i]);

		/*for (int j = 0; j < s->objs[i].faceNum; ++j) { // initial velocity
			s->objs[i].faces[j].v = vec3Copy(&s->objs[i].faces[j].normal);
			vec3Mul(&s->objs[i].faces[j].v, 0.0f);
		}*/
		for (int j = 0; j < s->objs[i].volNum; ++j) { // initial pressure
			s->objs[i].volumes[j].v = Vec3(0.0f, 0.0f, 0.0f);
			s->objs[i].volumes[j].p = 1.0f;
=======
		p_decomposeSurfs(&s->objs[i]);

		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			s->objs[i].faces[j].mFlux = vec3Copy(&s->objs[i].faces[j].normal);

			s->objs[i].faces[j].v = vec3Copy(&s->objs[i].faces[j].mFlux);
		}
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].p = (s->objs[i].volumes[j].centroid.x) + 1.0f;
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
		}
	}
}

void randomize(struct Sys *s)
{
	float intensity = 0.001f;

	for (int o = 0; o < s->objNum; ++o) {
		for (int i = 0; i < s->objs[o].mesh.vertNum; i += 4) {
			float nc = randomFloat() * 2.0f * M_PI;
			s->objs[o].mesh.vertData[i + 0] = s->objs[o].mesh.vertData[i + 0] * 1.0f + intensity * sinf(nc);
			s->objs[o].mesh.vertData[i + 1] = s->objs[o].mesh.vertData[i + 1] * 1.0f + intensity * cosf(nc);
			s->objs[o].mesh.vertData[i + 2] = s->objs[o].mesh.vertData[i + 2] * 1.0f + intensity * sinf(nc) * cosf(nc);
		}
	}
}

void p_updateVP(Obj *o)
{
	for (int j = 0; j < o->faceNum; ++j) {
		p_faceP(&o->faces[j]);
		p_faceV(&o->faces[j]);
	}
}

void p_updateM(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].mFlux = vec3Copy(&o->volumes[i].v);
		vec3Mul(&o->volumes[i].mFlux, o->fluid.rho);
	}
	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].mFlux = vec3Copy(&o->faces[i].v);
		vec3Mul(&o->faces[i].mFlux, o->fluid.rho);

		o->faces[i].mRate = fabs(vec3Dot(&o->faces[i].mFlux, &o->faces[i].surface));
	}
}

void p_sysTick(struct Sys *s)
{
	++s->ticks;
<<<<<<< HEAD
=======
	p_meshSetCol(&s->objs[0].mesh, 0.0f, 0.08f, 0.10f, 0.10f);

	s->t += s->dt;
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c

	for (int i = 0; i < s->objNum; ++i) {
		s->objs[i].dt = s->dt;

<<<<<<< HEAD
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].p = sinf(vec3Len(&s->objs[i].volumes[j].centroid) + s->objs[i].t*0.1f) * 20.1f;
		}

		p_updateVP(&s->objs[i]);

		p_updateM(&s->objs[i]);

		p_pGrad(&s->objs[i]);
		p_vGrad(&s->objs[i]);
		p_vFaceGrad(&s->objs[i]);

		p_computeFaceFs(&s->objs[i]);
		p_computeVolFs(&s->objs[i]);
		p_computeVCoeffs(&s->objs[i]);

		p_constructVMat(&s->objs[i]);

		s->objs[i].v = GaussSeidel(&s->objs[i].a, &s->objs[i].b);

		p_getV(&s->objs[i]);

#if 0
		matPrint(&s->objs[i].a);
		printf("XXXXXXXXXXXXXXXXXXXXXXX\n");
		matPrint(&s->objs[i].v);
		printf("=====================\n");
		matPrint(&s->objs[i].b);
		printf("\n");
#endif

		matDestroy(&s->objs[i].a);
		matDestroy(&s->objs[i].v);
		matDestroy(&s->objs[i].b);

		p_updateVP(&s->objs[i]);
		p_updateM(&s->objs[i]);
		p_vGrad(&s->objs[i]);
		p_vFaceGrad(&s->objs[i]);
		p_computeFaceFs(&s->objs[i]);
		//p_computeVolFs(&s->objs[i]);

		p_computePCoeffs(&s->objs[i]);

		p_constructPMat(&s->objs[i]);

		s->objs[i].v = GaussSeidel(&s->objs[i].a, &s->objs[i].b);

		p_getP(&s->objs[i]);

#if 0
		matPrint(&s->objs[i].a);
		printf("XXXXXXXXXXXXXXXXXXXXXXX\n");
		matPrint(&s->objs[i].v);
		printf("=====================\n");
		matPrint(&s->objs[i].b);
		printf("\n");
#endif
=======
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
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c

		matDestroy(&s->objs[i].a);
		matDestroy(&s->objs[i].v);
		matDestroy(&s->objs[i].b);
<<<<<<< HEAD

		s->objs[i].t += s->objs[i].dt;
=======
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
	}
}
