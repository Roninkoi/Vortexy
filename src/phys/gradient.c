//
// Created by rak on 4/10/20.
//

#include "gradient.h"

#define GRADIT 8 // gradient iterations
#define RCTRANS // rhie-chow transient term

int half = 0; // halfway approximation

// calculate geometric weighting factor
real getGWF(struct Face *f)
{
	vec3 d0 = vec3Copy(&f->thisVol[1]->r);

	vec3Sub(&d0, &f->r);

	return vec3Len(&d0) / vec3Len(&f->volDist);
}

// calculate geometric weighting factor
real getGWFh(struct Face *f)
{
	if (half)
		return 0.5;
	else
		return getGWF(f);
}

// calculate geometric weighting factor
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

	real g0 = getGWFh(f);

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

	real g0 = getGWFh(f);

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

	real g0 = getGWFh(f);

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
#if 0
	struct Volume *vol = f->thisVol[0]; // rhie-chow

	vec3 s = vec3Outwards(&vol->centroid,
						  &f->centroid,
						  &f->surface);
	s.x *= f->d.x;
	s.y *= f->d.y;
	s.z *= f->d.z; // s' == d s?

	vec3 t = vec3Outwards(&vol->centroid,
						  &f->centroid,
						  &f->surfaceT);

	t.x *= f->d.x;
	t.y *= f->d.y;
	t.z *= f->d.z;

	//f->p = vol->p;

	real pc = (vec3Dot(&vol->pGrad, &s) - vec3Dot(&f->pGrad, &t)) / f->df; // d_c = d_b = d_f
#else
	real pc = vec3Dot(&f->thisVol[0]->pGrad, &f->volDist); // taylor
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

// interpolated face pressure gradient
void p_pFaceGradI(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum < 2) {
			o->faces[i].pGradI = vec3Copy(&o->faces[i].thisVol[0]->pGrad);
		} else {
			vec3 g0 = vec3Copy(&o->faces[i].thisVol[0]->pGrad);
			vec3 g1 = vec3Copy(&o->faces[i].thisVol[1]->pGrad);

			real wf = getGWF(&o->faces[i]);

			vec3Mul(&g0, wf);
			vec3Mul(&g1, 1.0 - wf);
			vec3Add(&g0, &g1);

			o->faces[i].pGradI = vec3Copy(&g0);
		}
	}
}

// interpolated face velocity gradient
void p_vFaceGradI(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum < 2) {
			o->faces[i].vGradI = mat3Copy(&o->faces[i].thisVol[0]->vGrad);
		} else {
			mat3 g0 = mat3Copy(&o->faces[i].thisVol[0]->vGrad);
			mat3 g1 = mat3Copy(&o->faces[i].thisVol[1]->vGrad);

			real wf = getGWF(&o->faces[i]);

			mat3 gg0 = mat3Scale(&g0, wf);
			mat3 gg1 = mat3Scale(&g1, 1.0 - wf);

			o->faces[i].vGradI = mat3Add(&gg0, &gg1);
		}
	}
}

void p_pFaceGrad(Obj *o)
{
	p_pFaceGradI(o);

	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].pGrad = vec3Copy(&o->faces[i].pGradI);

		real op;

		if (o->faces[i].vNum < 2) {
			op = o->faces[i].p;
		}
		else {
			op = o->faces[i].thisVol[1]->p;
		}

		vec3 e = vec3Copy(&o->faces[i].volDist);
		vec3Normalize(&e);

		vec3 pc = vec3Copy(&e);

		real c = (op - o->faces[i].thisVol[0]->p) / vec3Len(&o->faces[i].volDist);
		c -= vec3Dot(&o->faces[i].pGradI, &e);

		vec3Mul(&pc, c);

		vec3Add(&o->faces[i].pGrad, &pc);
	}
}

void p_vFaceGrad(Obj *o)
{
	p_vFaceGradI(o);

	for (int i = 0; i < o->faceNum; ++i) {
		vec3 ov;

		if (o->faces[i].vNum < 2) {
			ov = vec3Copy(&o->faces[i].v);
		}
		else {
			ov = vec3Copy(&o->faces[i].thisVol[1]->v);
		}

		vec3 e = vec3Copy(&o->faces[i].volDist);
		vec3Normalize(&e);

		vec3 v0 = vec3Copy(&ov);
		vec3 v1 = vec3Copy(&o->faces[i].thisVol[0]->v);

		vec3Sub(&v0, &v1);
		vec3Div(&v0, vec3Len(&o->faces[i].volDist));

		vec3 vg1 = mat3DotV(&o->faces[i].vGradI, &e); // dot -> transpose
		vec3 vg2 = vec3Copy(&v0);
		vec3Sub(&vg2, &vg1); // v0 - vg1
		mat3 vg3 = mat3MulVV(&vg2, &e);
		o->faces[i].vGrad = mat3Add(&o->faces[i].vGradI, &vg3);
	}
}

// volume pressure gradient (Green-Gauss)
void p_pGrad(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		vec3 grad = nvec3();

		for (int j = 0; j < 4; ++j) {
			vec3 s = vec3Outwards(&o->volumes[i].r,
								  &o->volumes[i].faces[j]->r,
								  &o->volumes[i].faces[j]->surface);

			vec3Mul(&s, o->volumes[i].faces[j]->p);
			vec3Add(&grad, &s);
		}

		o->volumes[i].pGrad = vec3Copy(&grad);
		vec3Div(&o->volumes[i].pGrad, o->volumes[i].vol);
	}
}

// volume velocity gradient
void p_vGrad(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		mat3 *grad = &o->volumes[i].vGrad;

		*grad = nmat3();

		for (int j = 0; j < 4; ++j) {
			vec3 s = vec3Outwards(&o->volumes[i].r,
								  &o->volumes[i].faces[j]->r,
								  &o->volumes[i].faces[j]->surface);

			mat3 g = mat3MulVV(&s, &o->volumes[i].faces[j]->v);
			*grad = mat3Add(grad, &g);
		}

		*grad = mat3Scale(grad, 1.0 / o->volumes[i].vol);
	}
}

// volume pressure correction gradient
void p_pcGrad(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		vec3 grad = nvec3();

		for (int j = 0; j < 4; ++j) {
			vec3 s = vec3Outwards(&o->volumes[i].r,
								  &o->volumes[i].faces[j]->r,
								  &o->volumes[i].faces[j]->surface);
			vec3Mul(&s, o->volumes[i].faces[j]->pc);
			vec3Add(&grad, &s);
		}

		o->volumes[i].pcGrad = vec3Copy(&grad);
		vec3Div(&o->volumes[i].pcGrad, o->volumes[i].vol);
	}
}

// volume pressure gradient
void p_pGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceP(f);
	}

	p_pGrad(o);

	for (int k = 0; k < GRADIT; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue; // lower order estimate at boundary?
			}

			real wf = getGWFp(f);

			vec3 gc0 = vec3Copy(&f->thisVol[0]->pGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pGrad);

			vec3 rf = vec3Copy(&f->r);

			vec3 rc0 = vec3Copy(&rf);
			vec3 rc1 = vec3Copy(&rf);

			vec3Sub(&rc0, &f->thisVol[0]->r); // rf - rc
			vec3Sub(&rc1, &f->thisVol[1]->r);

			real c0 = wf * (vec3Dot(&gc0, &rc0)); // wf gc0 . (rf - rc0)
			real c1 = (1.0 - wf) * (vec3Dot(&gc1, &rc1));

			f->p = f->pI + c0 + c1;
		}

		p_pGrad(o);
	}
}

// volume velocity gradient
void p_vGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceV(f, 0);
	}

	p_vGrad(o);

	for (int k = 0; k < GRADIT; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue;
			}

			real wf = getGWFp(f);

			vec3 rf = vec3Copy(&f->r);

			vec3 rc0 = vec3Copy(&rf);
			vec3 rc1 = vec3Copy(&rf);

			vec3Sub(&rc0, &f->thisVol[0]->r); // rf - rc
			vec3Sub(&rc1, &f->thisVol[1]->r);

			vec3 gr0 = mat3DotV(&f->thisVol[0]->vGrad, &rc0);
			vec3 gr1 = mat3DotV(&f->thisVol[1]->vGrad, &rc1);

			vec3Mul(&gr0, wf);
			vec3Mul(&gr1, (1.0 - wf));

			f->v = vec3Copy(&f->vI);
			vec3Add(&f->v, &gr0);
			vec3Add(&f->v, &gr1);
		}

		p_vGrad(o);
	}
}

// pressure correction gradient
void p_pcGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_facePC(f);
	}

	p_pcGrad(o);

	for (int k = 0; k < GRADIT; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue;
			}

			real wf = getGWFp(f);

			vec3 gc0 = vec3Copy(&f->thisVol[0]->pcGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pcGrad);

			vec3 rf = vec3Copy(&f->r);

			vec3 rc0 = vec3Copy(&rf);
			vec3 rc1 = vec3Copy(&rf);

			vec3Sub(&rc0, &f->thisVol[0]->r); // rf - rc
			vec3Sub(&rc1, &f->thisVol[1]->r);

			real c0 = wf * (vec3Dot(&gc0, &rc0)); // wf gc0 . (rf - rc0)
			real c1 = (1.0 - wf) * (vec3Dot(&gc1, &rc1));

			f->pc = f->pcI + c0 + c1;
		}

		p_pcGrad(o);
	}
}

// volume pressure gradient halfway
void p_pGradsh(Obj *o)
{
	half = 1;
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceP(f);
	}
	half = 0;

	p_pGrad(o);

	for (int k = 0; k < GRADIT; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue; // lower order estimate at boundary?
			}

			vec3 gc0 = vec3Copy(&f->thisVol[0]->pGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pGrad);

			vec3Add(&gc0, &gc1);

			vec3 rf = vec3Copy(&f->r);

			vec3 rcf = vec3Copy(&f->thisVol[0]->r);
			vec3Add(&rcf, &f->thisVol[1]->r);
			vec3Mul(&rcf, 0.5);

			vec3Sub(&rf, &rcf);

			real c = 0.5 * vec3Dot(&gc0, &rf); // wf gc0 . (rf - rc0)

			f->p = f->pI + c;
		}

		p_pGrad(o);
	}
}

// volume velocity gradient halfway
void p_vGradsh(Obj *o)
{
	half = 1;
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceV(f, 0);
	}
	half = 0;

	p_vGrad(o);

	for (int k = 0; k < GRADIT; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue;
			}

			mat3 gc0 = mat3Copy(&f->thisVol[0]->vGrad);
			mat3 gc1 = mat3Add(&gc0, &f->thisVol[1]->vGrad); // gc0 = vg0 + vg1

			vec3 rf = vec3Copy(&f->r);

			vec3 rcf = vec3Copy(&f->thisVol[0]->r);
			vec3Add(&rcf, &f->thisVol[1]->r);
			vec3Mul(&rcf, 0.5);

			vec3Sub(&rf, &rcf);

			vec3 d = mat3DotV(&gc0, &rf);
			vec3Mul(&d, 0.5);

			f->v = vec3Copy(&f->vI);
			vec3Add(&f->v, &d);
		}

		p_vGrad(o);
	}
}

// pressure correction gradient halfway
void p_pcGradsh(Obj *o)
{
	half = 1;
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_facePC(f);
	}
	half = 0;

	p_pcGrad(o);

	for (int k = 0; k < GRADIT; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue;
			}

			vec3 gc0 = vec3Copy(&f->thisVol[0]->pcGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pcGrad);

			vec3Add(&gc0, &gc1);

			vec3 rf = vec3Copy(&f->r);

			vec3 rcf = vec3Copy(&f->thisVol[0]->r);
			vec3Add(&rcf, &f->thisVol[1]->r);
			vec3Mul(&rcf, 0.5);

			vec3Sub(&rf, &rcf);

			real c = 0.5 * vec3Dot(&gc0, &rf); // wf gc0 . (rf - rc0)

			f->pc = f->pcI + c;
		}

		p_pcGrad(o);
	}
}
