#include <string.h>
#include <util/algebra.h>

#include "util/randomUtil.h"
#include "sys.h"
#include "util/mat.h"

#define printVField(prefix, postfix0, postfix1, pfn)    \
    for (int pfi = 0; pfi < pfn; ++pfi) {                \
        vec3Prints(&(prefix[pfi].postfix0));            \
        vec3Print(&(prefix[pfi].postfix1));                \
    }

#define printSField(prefix, postfix0, postfix1, pfn)    \
    for (int pfi = 0; pfi < pfn; ++pfi) {                \
        vec3Prints(&(prefix[pfi].postfix0));            \
        printf("%f\n", (prefix[pfi].postfix1));            \
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

void p_setInitial(Obj *o)
{
	for (int j = 0; j < o->faceNum; ++j) { // initial conditions
		vec3 v0 = vec3Copy(&o->faces[j].normal);
		vec3Mul(&v0, o->faces[j].initialV);

		o->faces[j].v = vec3Copy(&v0);
		o->faces[j].p = o->faces[j].initialP;

		o->faces[j].c = nvec3();
		o->faces[j].pGradI = nvec3();
		o->faces[j].pGrad = nvec3();
	}
}

void p_setBoundary(Obj *o)
{
	for (int j = 0; j < o->faceNum; ++j) { // boundary conditions
		if (o->faces[j].boundary == 0)
			continue;

		vec3 cv;
		switch (o->faces[j].boundary) {
			case 3:
				cv = vec3Copy(&o->faces[j].normal);
				vec3Mul(&cv, o->faces[j].constantV);
				o->faces[j].v = vec3Copy(&cv);
				break;
			case 4:
				o->faces[j].p = o->faces[j].constantP;
				break;
		}
	}
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

// update velocity and pressure at faces
void p_updateVP(Obj *o, int rc)
{
	for (int j = 0; j < o->faceNum; ++j) {
		p_faceP(&o->faces[j]);
		p_faceVE(&o->faces[j], rc);
	}
}

void p_updatePC(Obj *o)
{
	for (int j = 0; j < o->faceNum; ++j) {
		p_facePC(&o->faces[j]);
	}
}

void p_correctRC(Obj *o)
{
	for (int j = 0; j < o->faceNum; ++j) {
#if 1
		p_faceVRCE(&o->faces[j], o->vRelax);
#else
		p_faceVRC(&o->faces[j]);
#endif
	}
}

// save old iteration values
void p_in(Obj *o)
{
	for (int j = 0; j < o->volNum; ++j) {
		o->volumes[j].vn = vec3Copy(&o->volumes[j].v);
		o->volumes[j].pGradn = vec3Copy(&o->volumes[j].pGrad);
	}
	for (int j = 0; j < o->faceNum; ++j) {
		o->faces[j].vn = vec3Copy(&o->faces[j].v);
		o->faces[j].vIn = vec3Copy(&o->faces[j].vI);
		o->faces[j].pGradn = vec3Copy(&o->faces[j].pGrad);
	}
}

// save old time values
void p_tn(Obj *o)
{
	for (int j = 0; j < o->volNum; ++j) {
		o->volumes[j].vtn = vec3Copy(&o->volumes[j].v);
	}
	for (int j = 0; j < o->faceNum; ++j) {
		o->faces[j].vtn = vec3Copy(&o->faces[j].v);
		o->faces[j].vItn = vec3Copy(&o->faces[j].vI);
		o->faces[j].ctn = vec3Copy(&o->faces[j].c);
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

void p_sysStart(struct Sys *s)
{
	gradIt = s->gradIt;
	transient = s->transient;
	convsch = s->convsch;

	for (int i = 0; i < s->objNum; ++i) {
		s->objs[i].t = 0.0;

		p_decomposeSurfs(&s->objs[i]);

		p_setInitial(&s->objs[i]);

		for (int j = 0; j < s->objs[i].volNum; ++j) {
			s->objs[i].volumes[j].c = nvec3();
			s->objs[i].volumes[j].pGrad = nvec3();
			s->objs[i].volumes[j].pcGrad = nvec3();

			volV(&s->objs[i].volumes[j]);
			volP(&s->objs[i].volumes[j]);
		}

		s->objs[i].vx = Matc(1.0, s->objs[i].volNum);
		s->objs[i].vy = Matc(1.0, s->objs[i].volNum);
		s->objs[i].vz = Matc(1.0, s->objs[i].volNum);
		s->objs[i].vp = Matc(1.0, s->objs[i].volNum);

		p_in(&s->objs[i]);
		p_tn(&s->objs[i]);
	}
}

void p_sysStatus(struct Sys *s)
{
	printf("residual %.4e, in %i\n", s->res, s->in);

	printf("max solver iterations: %i\n", msiterations);
	msiterations = 0;

	for (int i = 0; i < s->objNum; ++i)
		printf("o %i, t (s): %f\n", i, s->objs[i].t);

	if (!msconvergence) {
		msconvergence = 1;

		printf("Matrix solver not converging!\n");
	}

	if (s->in >= s->dtMaxIt && s->res > s->residual && s->residual > 0.0) {
		printf("Solution not converging!\n");
	}

	if (s->unreal) {
		printf("Solution not physical (code: %i)\n", s->unreal);
		s->unreal = 0;
	}
}

void p_sysTick(struct Sys *s)
{
	for (int i = 0; i < s->objNum; ++i) {
		s->in = 0;

		do { // SIMPLE algorithm
			if (s->gradIt > 0) {
				p_pGradsh(&s->objs[i]);
				p_vGradsh(&s->objs[i]);
			} else {
				p_updateVP(&s->objs[i], 0);

				p_pGrad(&s->objs[i]);
				p_vGrad(&s->objs[i]);
			}

			p_pFaceGrad(&s->objs[i]);
			p_vFaceGrad(&s->objs[i]);

			p_computeVolFs(&s->objs[i]);
			p_computeFaceFs(&s->objs[i], s->in, floor(s->objs[i].t / s->objs[i].dt));

			p_computeVCoeffs(&s->objs[i]);

			p_computeC(&s->objs[i]);

			p_faceBoundP(&s->objs[i]); // extrapolate pressure at boundary faces?

			p_constructVMatX(&s->objs[i]);
			solve(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vx, s->maxIt, s->epsilon);
			p_getVX(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			p_constructVMatY(&s->objs[i]);
			solve(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vy, s->maxIt, s->epsilon);
			p_getVY(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			p_constructVMatZ(&s->objs[i]);
			solve(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vz, s->maxIt, s->epsilon);
			p_getVZ(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			p_updateVP(&s->objs[i], 1);

			p_computePCoeffs(&s->objs[i]);
			p_computePBoundCoeffs(&s->objs[i]);

			p_constructPMat(&s->objs[i]);
			solve(&s->objs[i].a, &s->objs[i].b, &s->objs[i].vp, s->maxIt, s->epsilon);
			p_getP(&s->objs[i]);

			matDestroyS(&s->objs[i].a);
			matDestroy(&s->objs[i].b);

			if (s->gradIt > 0) {
				p_pcGradsh(&s->objs[i]);
			} else {
				p_pcGrad(&s->objs[i]);
			}

			s->res = 0.0;
			real avgP = 0.0;

			// apply field corrections
			for (int j = 0; j < s->objs[i].volNum; ++j) {
				avgP += s->objs[i].volumes[j].p;

				s->objs[i].volumes[j].p += s->objs[i].volumes[j].pc * s->objs[i].pRelax;

				real corr = fabs(s->objs[i].volumes[j].pc);

				if (s->relres) {
					s->res += corr;
				}
				else if (corr > s->res) {
					s->res = corr;
				}

				s->objs[i].volumes[j].v.x -=
						s->objs[i].volumes[j].c.x * s->objs[i].volumes[j].pcGrad.x * s->relaxm;
				s->objs[i].volumes[j].v.y -=
						s->objs[i].volumes[j].c.y * s->objs[i].volumes[j].pcGrad.y * s->relaxm;
				s->objs[i].volumes[j].v.z -=
						s->objs[i].volumes[j].c.z * s->objs[i].volumes[j].pcGrad.z * s->relaxm;

				if (s->objs[i].volumes[j].p < 0.0) {
					s->objs[i].volumes[j].p = 0.0;
					s->unreal |= 4;
				}
			}

			avgP /= s->objs[i].volNum;

			if (s->relres)
				s->res /= avgP;

			//printSField(s->objs[i].volumes, r, p, s->objs[i].volNum);

			p_in(&s->objs[i]);
			++s->in;

			if (s->printitn)
				printf("itn %i / %i, res %.4e, msi %i\n", s->in, s->dtMaxIt, s->res, msiterations);

			//solver not converging
			if (s->msdivhalt && !msconvergence) {
				s->simulating = 0;
				break;
			}

			if (simkill)
				break;
		} while (s->res > s->residual && s->in < s->dtMaxIt);

		p_tn(&s->objs[i]);
		s->objs[i].t += s->objs[i].dt;

		// solution not converging
		if (s->in >= s->dtMaxIt && s->residual > 0.0 && s->res > s->residual && s->divhalt)
			s->simulating = 0; // stop

		if (s->objs[i].t > s->endt) // end?
			s->simulating = 0;
	}

	++s->ticks;
}
