#include <string.h>
#include <util/algebra.h>

#include "util/randomUtil.h"
#include "sys.h"
#include "util/mat.h"

void p_sysInit(struct Sys *s)
{
	s->objs = NULL;
	s->objNum = 0;
	s->simulating = 1;

	s->t = 0.0f;
}

void p_addObj(struct Sys *s, char *fluidPath)
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

	p_loadObj(&s->objs[oldNum], fluidPath);
}

void p_sysStart(struct Sys *s)
{
	s->selected = 0;
	for (int i = 0; i < s->objNum; ++i) {
		s->objs[i].f = Vec3(0.0f, 0.0f, 0.0f);
		s->objs[i].t = 0.0f;

		p_decomposeSurfs(&s->objs[i]);

		for (int j = 0; j < s->objs[i].faceNum; ++j) { // initial conditions
			vec3 v0 = vec3Copy(&s->objs[i].faces[j].normal);
			vec3Mul(&v0, s->objs[i].faces[j].initialV);
			
			s->objs[i].faces[j].v = vec3Copy(&v0);
			s->objs[i].faces[j].p = s->objs[i].faces[j].initialP;
		}

		for (int j = 0; j < s->objs[i].volNum; ++j) {
			p_volV(&s->objs[i].volumes[j]);
			p_volP(&s->objs[i].volumes[j]);
		}
	}
}

// update velocity and pressure at faces
void p_updateVP(Obj *o)
{
	for (int j = 0; j < o->faceNum; ++j) {
		p_faceP(&o->faces[j]);
		p_faceV(&o->faces[j]);
	}
}

// update mass flow
void p_updateM(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].mFlux = vec3Copy(&o->volumes[i].v);
		vec3Mul(&o->volumes[i].mFlux, o->fluid.rho);
	}
	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].mFlux = vec3Copy(&o->faces[i].v);
		vec3Mul(&o->faces[i].mFlux, o->fluid.rho);

		o->faces[i].mRate = (vec3Dot(&o->faces[i].mFlux, &o->faces[i].surface));
	}
}

// main simulation loop
void p_sysTick(struct Sys *s)
{
	++s->ticks;
	s->t += s->dt;

	for (int i = 0; i < s->objNum; ++i) {
		s->objs[i].dt = s->dt;
		s->objs[i].pRelax = s->pRelax;
		s->objs[i].vRelax = s->vRelax;

		p_updateVP(&s->objs[i]);

		p_updateM(&s->objs[i]);

		p_pGrad(&s->objs[i]);
		p_vGrad(&s->objs[i]);
		//p_pFaceGrad(&s->objs[i]);
		p_vFaceGrad(&s->objs[i]);

		p_computeFaceFs(&s->objs[i]);
		p_computeVolFs(&s->objs[i]);
		p_computeVCoeffs(&s->objs[i]);

		p_constructVMat(&s->objs[i]);

		s->objs[i].v = GaussSeidel(&s->objs[i].a, &s->objs[i].b, s->maxIt, s->epsilon);

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
		/*p_vGrad(&s->objs[i]);
		p_vFaceGrad(&s->objs[i]);
		p_computeFaceFs(&s->objs[i]);*/
		//p_computeVolFs(&s->objs[i]);

		p_computePCoeffs(&s->objs[i]);

		p_constructPMat(&s->objs[i]);

		s->objs[i].v = GaussSeidel(&s->objs[i].a, &s->objs[i].b, s->maxIt, s->epsilon);

		p_getP(&s->objs[i]);

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

		s->objs[i].t += s->objs[i].dt;
	}

	if (s->t > s->endt)
		s->simulating = 0;
}
