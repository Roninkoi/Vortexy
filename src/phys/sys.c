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
	s->reset = 0;
	s->selected = 0;
}

void p_addObj(struct Sys *s, char *fluidPath, int mode)
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

	p_loadObj(&s->objs[oldNum], fluidPath, mode);
}

void p_sysEnd(struct Sys *s)
{
	for (int i = 0; i < s->objNum; ++i) {
		matDestroy(&s->objs[i].vx);
		matDestroy(&s->objs[i].vy);
		matDestroy(&s->objs[i].vz);
		matDestroy(&s->objs[i].vp);

		p_destroyObj(&s->objs[i]);
	}
}

// volume velocity from faces
void volV(struct Volume *v)
{
	v->v = nvec3();
	for (int i = 0; i < 4; ++i) {
		vec3Add(&v->v, &v->faces[i]->v);
	}
}

// volume pressure from faces
void volP(struct Volume *v)
{
	v->p = 0.0f;
	for (int i = 0; i < 4; ++i) {
		v->p += v->faces[i]->p;
	}
	v->p /= 4.0f;
}

void p_sysRestart(struct Sys *s, char *fluidPath)
{
	for (int i = 0; i < s->objNum; ++i) {
		matDestroy(&s->objs[i].vx);
		matDestroy(&s->objs[i].vy);
		matDestroy(&s->objs[i].vz);
		matDestroy(&s->objs[i].vp);

		p_reloadObj(&s->objs[i], fluidPath);
	}

	p_sysStart(s);
}

void p_sysStart(struct Sys *s)
{
	for (int i = 0; i < s->objNum; ++i) {
		s->objs[i].t = 0.0f;

		p_decomposeSurfs(&s->objs[i]);

		for (int j = 0; j < s->objs[i].faceNum; ++j) { // initial conditions
			vec3 v0 = vec3Copy(&s->objs[i].faces[j].normal);
			vec3Mul(&v0, s->objs[i].faces[j].initialV);

			s->objs[i].faces[j].v = vec3Copy(&v0);
			s->objs[i].faces[j].p = s->objs[i].fluid.bp + s->objs[i].faces[j].initialP;

			s->objs[i].faces[j].d = nvec3();
			s->objs[i].faces[j].pGradI = nvec3();
			s->objs[i].faces[j].pGrad = nvec3();
		}

		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].d = nvec3();
			s->objs[i].volumes[j].pGrad = nvec3();
			s->objs[i].volumes[j].pcGrad = nvec3();

			volV(&s->objs[i].volumes[j]);
			volP(&s->objs[i].volumes[j]);
		}

		s->objs[i].vx = Mat(1.0f, s->objs[i].volNum, 1);
		s->objs[i].vy = Mat(1.0f, s->objs[i].volNum, 1);
		s->objs[i].vz = Mat(1.0f, s->objs[i].volNum, 1);
		s->objs[i].vp = Mat(1.0f, s->objs[i].volNum, 1);
	}
}

// update velocity and pressure at faces
void p_updateVP(Obj *o, int rc)
{
	for (int j = 0; j < o->faceNum; ++j) {
		p_faceP(&o->faces[j]);
		p_faceV(&o->faces[j], rc);
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
	for (int i = 0; i < s->objNum; ++i) {
		float criterion = 10000.0f;
		float residual = 2.0f * criterion;

		do {
			p_updateVP(&s->objs[i], 0);
			p_updateM(&s->objs[i]);

			p_pGrad(&s->objs[i]);
			p_vGrad(&s->objs[i]);
			p_pFaceGrad(&s->objs[i]);
			p_vFaceGrad(&s->objs[i]);

			p_computeVolFs(&s->objs[i]);
			p_computeFaceFs(&s->objs[i]);

			p_computeVCoeffs(&s->objs[i]);
			p_computeVFaceCoeffs(&s->objs[i]);

			p_computeD(&s->objs[i]);

			p_computeVBoundCoeffs(&s->objs[i]);

			p_constructVMatX(&s->objs[i]);
			GaussSeidelSG(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vx, s->maxIt, s->epsilon);
			p_getVX(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			p_constructVMatY(&s->objs[i]);
			GaussSeidelSG(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vy, s->maxIt, s->epsilon);
			p_getVY(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			p_constructVMatZ(&s->objs[i]);
			GaussSeidelSG(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vz, s->maxIt, s->epsilon);
			p_getVZ(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			p_updateVP(&s->objs[i], 1);
			p_updateM(&s->objs[i]);

			p_computePCoeffs(&s->objs[i]);

			p_computePBoundCoeffs(&s->objs[i]);

			p_constructPMat(&s->objs[i]);
			GaussSeidelSG(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vp, s->maxIt, s->epsilon);
			p_getP(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			p_pcGrad(&s->objs[i]);

			residual = 0.0f;

			// apply field corrections
			for (int j = 0; j < s->objs[i].volNum; ++j) {
				s->objs[i].volumes[j].p += s->objs[i].volumes[j].pc * s->objs[i].pRelax;

				residual += fabs(s->objs[i].volumes[j].pc * s->objs[i].pRelax);

#if 0
				s->objs[i].volumes[j].v.x -= s->objs[i].volumes[j].d.x * s->objs[i].volumes[j].pcGrad.x * s->objs[i].pRelax;
				s->objs[i].volumes[j].v.y -= s->objs[i].volumes[j].d.y * s->objs[i].volumes[j].pcGrad.y * s->objs[i].pRelax;
				s->objs[i].volumes[j].v.z -= s->objs[i].volumes[j].d.z * s->objs[i].volumes[j].pcGrad.z * s->objs[i].pRelax;
#else
				s->objs[i].volumes[j].v.x -= s->objs[i].volumes[j].d.x * s->objs[i].volumes[j].pcGrad.x;
				s->objs[i].volumes[j].v.y -= s->objs[i].volumes[j].d.y * s->objs[i].volumes[j].pcGrad.y;
				s->objs[i].volumes[j].v.z -= s->objs[i].volumes[j].d.z * s->objs[i].volumes[j].pcGrad.z;
#endif

				if (s->objs[i].volumes[j].p < 0.0f)
					s->objs[i].volumes[j].p = 0.0f;
			}
		} while (residual > criterion && false);

		s->objs[i].t += s->objs[i].dt;

		if (s->objs[i].t > s->endt)
			s->simulating = 0;
	}

	++s->ticks;
}
