//
// Created by rak on 6/4/20.
//

#include "interpolate.h"

#define RCTRANS // rhie-chow transient term

// calculate geometric weighting factor
real getGWF(struct Face *f)
{
	vec3 d0 = vec3Copy(&f->thisVol[1]->r);

	vec3Sub(&d0, &f->r);

	return vec3Len(&d0) / vec3Len(&f->volDist);
}

// calculate geometric weighting factor halfway
real getGWFh(struct Face *f)
{
	if (halfGWF)
		return 0.5;
	else
		return getGWF(f);
}

// calculate geometric weighting factor with extrapolation
real getGWFp(struct Face *f)
{
	vec3 e = vec3Copy(&f->volDist);
	vec3Normalize(&e);

	vec3 fc = vec3Copy(&e);
	real fd = vec3Dot(&f->r, &f->normal) / vec3Dot(&e, &f->normal);
	vec3Mul(&fc, fd);

	vec3 d0 = vec3Copy(&f->thisVol[1]->r);

	vec3Sub(&d0, &fc);

	return vec3Len(&d0) / vec3Len(&f->volDist);
}

void p_faceD(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->c = f->thisVol[0]->c;
		return;
	}

	real g0 = getGWF(f);

	vec3 pd0 = vec3Copy(&f->thisVol[0]->c);
	vec3Mul(&pd0, g0);

	vec3 pd1 = vec3Copy(&f->thisVol[1]->c);
	vec3Mul(&pd1, (1.0 - g0));

	f->c = vec3Copy(&pd0);
	vec3Add(&f->c, &pd1);
}

// interpolate face velocity from volumes
void p_faceVI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->vI = vec3Copy(&f->thisVol[0]->v);
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->r);

	vec3Sub(&d0, &f->r);

	real g0 = getGWF(f);

	vec3 vel0 = vec3Copy(&f->thisVol[0]->v);
	vec3Mul(&vel0, g0);

	vec3 vel1 = vec3Copy(&f->thisVol[1]->v);
	vec3Mul(&vel1, (1.0 - g0));

	f->vI = vec3Copy(&vel0);
	vec3Add(&f->vI, &vel1);
}

// interpolate face pressure from volumes
void p_facePI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->pI = f->thisVol[0]->p;
		return;
	}

	real g0 = getGWF(f);

	real pre0 = f->thisVol[0]->p * g0;

	real pre1 = f->thisVol[1]->p * (1.0 - g0);

	f->pI = pre0 + pre1;
}

// interpolate face pressure correction from volumes
void p_facePCI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->pc = f->thisVol[0]->pc;
		return;
	}

	real g0 = getGWF(f);

	real pre0 = f->thisVol[0]->pc * g0;

	real pre1 = f->thisVol[1]->pc * (1.0 - g0);

	f->pcI = pre0 + pre1;
}

void p_facePC(struct Face *f)
{
	p_facePCI(f);

	f->pc = f->pcI;
}

// extrapolate pressure to boundary
real boundaryPressure(struct Face *f)
{
#if 1
	struct Volume *vol = f->thisVol[0]; // rhie-chow

	vec3 s = vec3Outwards(&vol->r,
						  &f->r,
						  &f->surface);
	s.x *= f->c.x;
	s.y *= f->c.y;
	s.z *= f->c.z; // s' == c s?

	vec3 t = vec3Outwards(&vol->r,
						  &f->r,
						  &f->surfaceT);

	t.x *= f->c.x;
	t.y *= f->c.y;
	t.z *= f->c.z;

	real pc = (vec3Dot(&vol->pGradn, &s) - vec3Dot(&f->pGradn, &t)) / f->df;
#else
	real pc = vec3Dot(&f->thisVol[0]->pGradn, &f->volDist); // taylor
#endif

#if 0
	f->p = vol->p; // low-order
	pc = 0.0;
#endif

	return pc;
}

void p_faceBoundP(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		switch (f->boundary) {
			case 1:
			case 2:
			case 3: // same for 1, 2, 3
			case 10:
				f->p += boundaryPressure(f);
				return;
			case 4:
			case 6:
				f->p = f->constantP;
				return;
		}
	}
}

void p_faceP(struct Face *f)
{
	p_facePI(f);

	f->p = f->pI;

	switch (f->boundary) {
		case 4:
		case 6:
			f->p = f->constantP;
			return;
	}
}

// Rhie-Chow interpolation
void p_faceVRC(struct Face *f)
{
	f->v = vec3Copy(&f->vI);

	vec3 vc = vec3Copy(&f->pGrad);

	vec3Sub(&vc, &f->pGradI);

	vc.x *= f->c.x;
	vc.y *= f->c.y;
	vc.z *= f->c.z;

	vec3Sub(&f->v, &vc);
}

// Rhie-Chow interpolation extended
void p_faceVRCE(struct Face *f, real urf)
{
	f->v = vec3Copy(&f->vI);

	vec3 vc = vec3Copy(&f->pGrad);

	vec3Sub(&vc, &f->pGradI);

	vc.x *= f->c.x;
	vc.y *= f->c.y;
	vc.z *= f->c.z;

	vec3Sub(&f->v, &vc);

	vc = vec3Copy(&f->vn); // under-relaxation
	vec3Sub(&vc, &f->vIn);
	vec3Mul(&vc, 1.0 - urf);

	vec3Add(&f->v, &vc);

#ifndef RCTRANS
	return;
#endif

	vc = vec3Copy(&f->vtn); // transient (first-order euler only)
	vec3Sub(&vc, &f->vItn);

	if (f->ctn.x == 0.0 || f->ctn.y == 0.0 || f->ctn.z == 0.0)
		return;

	vc.x *= f->c.x / f->ctn.x;
	vc.y *= f->c.y / f->ctn.y;
	vc.z *= f->c.z / f->ctn.z;

	vec3Add(&f->v, &vc);
}

void fv(struct Face *f)
{
	p_faceVI(f);

	f->v = vec3Copy(&f->vI);

	return;

	vec3 cv;
	switch (f->boundary) {
		case 1:
			f->v = nvec3();
			return;
		case 3:
			cv = vec3Copy(&f->normal);
			vec3Mul(&cv, f->constantV);
			f->v = vec3Copy(&cv);
			return;
		case 10:
			f->v = Vec3(f->constantV, 0.0, 0.0);
			return;
	}
}

void p_faceV(struct Face *f, int rc)
{
	fv(f);

	if (rc)
		p_faceVRC(f);
}

// if urf == 0.0 then rc = 0
void p_faceVE(struct Face *f, real urf)
{
	fv(f);

	if (urf > 0.0)
		p_faceVRCE(f, urf);
}

