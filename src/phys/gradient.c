//
// Created by rak on 4/10/20.
//

#include "gradient.h"

//#define HALF // halfway approximation
#define GRADIT 2

// calculate geometric weighting factor
real getGWF(struct Face *f)
{
	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);

#ifdef HALF
	return 0.5;
#endif

	return vec3Len(&d0) / vec3Len(&f->volDist);
}

// interpolate face velocity from volumes
void p_faceVI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->vi = vec3Copy(&f->thisVol[0]->v);
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);

	real g0 = getGWF(f);

	vec3 vel0 = vec3Copy(&f->thisVol[0]->v);
	vec3Mul(&vel0, g0);

	vec3 vel1 = vec3Copy(&f->thisVol[1]->v);
	vec3Mul(&vel1, (1.0 - g0));

	f->vi = vec3Copy(&vel0);
	vec3Add(&f->vi, &vel1);
}

// interpolate face pressure from volumes
void p_facePI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		real e = vec3Dot(&f->thisVol[0]->pGrad, &f->volDist); // extrapolation?
		f->pi = f->thisVol[0]->p;
		return;
	}

	real g0 = getGWF(f);

	real pre0 = f->thisVol[0]->p * g0;

	real pre1 = f->thisVol[1]->p * (1.0 - g0);

	f->pi = pre0 + pre1;
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

	f->pci = pre0 + pre1;
}

void p_facePC(struct Face *f)
{
	p_facePCI(f);

	f->pc = f->pci;
}

void p_faceP(struct Face *f)
{
	p_facePI(f);

	f->p = f->pi;
}

// Rhie-Chow interpolation
void p_faceVRC(struct Face *f)
{
	f->v = vec3Copy(&f->vi);
	
	vec3 vc = vec3Copy(&f->pGrad);

	vec3Sub(&vc, &f->pGradI);

	vc.x *= f->d.x;
	vc.y *= f->d.y;
	vc.z *= f->d.z;

	vec3Sub(&f->v, &vc);
}

// Rhie-Chow interpolation extended
void p_faceVRCE(struct Face *f, real urf)
{
	f->v = vec3Copy(&f->vi);

	vec3 vc = vec3Copy(&f->pGrad);

	vec3Sub(&vc, &f->pGradI);

	vc.x *= f->d.x;
	vc.y *= f->d.y;
	vc.z *= f->d.z;

	vec3Sub(&f->v, &vc);

	vc = vec3Copy(&f->vn);

	vec3Sub(&vc, &f->vin);
	vec3Mul(&vc, 1.0 - urf);

	vec3Add(&f->v, &vc);
}

void p_faceV(struct Face *f, int rc)
{
	f->vin = vec3Copy(&f->vi);
	f->vn = vec3Copy(&f->v);

	p_faceVI(f);

	f->v = vec3Copy(&f->vi);

	if (rc)
		p_faceVRC(f);
}

void p_faceVE(struct Face *f, float urf)
{
	f->vin = vec3Copy(&f->vi);
	f->vn = vec3Copy(&f->v);

	p_faceVI(f);

	f->v = vec3Copy(&f->vi);

	if (urf > 0.0)
		p_faceVRCE(f, urf);
}

// interpolated face pressure gradient
void p_pFaceGradI(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum < 2 ) {
			o->faces[i].pGradI = vec3Copy(&o->faces[i].thisVol[0]->pGrad);
		} else {
			vec3 g0 = vec3Copy(&o->faces[i].thisVol[0]->pGrad);
			vec3 g1 = vec3Copy(&o->faces[i].thisVol[1]->pGrad);

			vec3 d0 = vec3Copy(&o->faces[i].thisVol[1]->centroid);

			vec3Sub(&d0, &o->faces[i].centroid);
			real wf = vec3Len(&d0) / vec3Len(&o->faces[i].volDist);

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
			matDestroy(&o->faces[i].vGradI);
			o->faces[i].vGradI = matCopy(&o->faces[i].thisVol[0]->vGrad);
		} else {
			matDestroy(&o->faces[i].vGradI);
			mat g0 = matCopy(&o->faces[i].thisVol[0]->vGrad);
			mat g1 = matCopy(&o->faces[i].thisVol[1]->vGrad);

			vec3 d0 = vec3Copy(&o->faces[i].thisVol[1]->centroid);
			vec3Sub(&d0, &o->faces[i].centroid);

			real wf = vec3Len(&d0) / vec3Len(&o->faces[i].volDist);

			mat s0 = Mat(wf, 3, 3);
			mat s1 = Mat(1.0 - wf, 3, 3);

			mat gg0 = matMul(&g0, &s0);
			mat gg1 = matMul(&g1, &s1);

			o->faces[i].vGradI = matAdd(&gg0, &gg1);

			matDestroy(&g0);
			matDestroy(&g1);
			matDestroy(&gg0);
			matDestroy(&gg1);
			matDestroy(&s0);
			matDestroy(&s1);
		}
	}
}

void p_pFaceGrad(Obj *o)
{
	p_pFaceGradI(o);

	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].pGrad = vec3Copy(&o->faces[i].pGradI);

		if (o->faces[i].vNum < 2)
			continue;

		vec3 e = vec3Copy(&o->faces[i].volDist);
		vec3Normalize(&e);

		vec3 pc = vec3Copy(&e);

		real c = (o->faces[i].thisVol[1]->p - o->faces[i].thisVol[0]->p) / vec3Len(&o->faces[i].volDist);
		c -= vec3Dot(&o->faces[i].pGradI, &e);

		vec3Mul(&pc, c);

		vec3Add(&o->faces[i].pGrad, &pc);
	}
}

void p_vFaceGrad(Obj *o)
{
	p_vFaceGradI(o);

	for (int i = 0; i < o->faceNum; ++i) {
		matDestroy(&o->faces[i].vGrad);

		if (o->faces[i].vNum < 2) {
			o->faces[i].vGrad = matCopy(&o->faces[i].vGradI);

			continue;
		}

		vec3 e = vec3Copy(&o->faces[i].volDist);

		vec3 v0 = vec3Copy(&o->faces[i].thisVol[1]->v);
		vec3 v1 = vec3Copy(&o->faces[i].thisVol[0]->v);

		vec3Sub(&v0, &v1);
		vec3Mul(&v0, 1.0 / vec3Len(&e));
		mat vm = matVec3(&v0);

		vec3Normalize(&e);

		mat vg0 = matCopy(&o->faces[i].vGradI);
		mat em = matVec3(&e);
		mat emt = matTranspose(&em);
		mat vg1 = matMul(&vg0, &em);

		mat vg2 = matSub(&vm, &vg1);
		mat vg3 = matMul(&vg2, &emt);

		o->faces[i].vGrad = matAdd(&o->faces[i].vGradI, &vg3);

		matDestroy(&vm);
		matDestroy(&vg0);
		matDestroy(&em);
		matDestroy(&emt);
		matDestroy(&vg1);
		matDestroy(&vg2);
		matDestroy(&vg3);
	}
}

// volume pressure gradient
void p_pGrad(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		vec3 grad = nvec3();

		for (int j = 0; j < 4; ++j) {
			vec3 s = vec3Outwards(&o->volumes[i].centroid,
								  &o->volumes[i].faces[j]->centroid,
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
		mat *grad = &o->volumes[i].vGrad;
		matDestroy(grad);
		*grad = Mat(0.0, 3, 3);

		for (int j = 0; j < 4; ++j) {
			vec3 s = vec3Outwards(&o->volumes[i].centroid,
								  &o->volumes[i].faces[j]->centroid,
								  &o->volumes[i].faces[j]->surface);

			mat g0 = matVec3(&s);
			mat g1 = matVec3(&o->volumes[i].faces[j]->v);
			mat g2 = matTranspose(&g1);

			mat g = matMul(&g0, &g2);

			mat a = matAdd(grad, &g);
			matDestroy(grad);
			*grad = matCopy(&a);

			matDestroy(&g0);
			matDestroy(&g1);
			matDestroy(&g2);
			matDestroy(&g);
			matDestroy(&a);
		}

		mat s = Mat(1.0 / o->volumes[i].vol, 3, 3);
		mat a = matMul(grad, &s);
		matDestroy(&s);
		matDestroy(grad);

		*grad = matCopy(&a);
		matDestroy(&a);
	}
}

// volume pressure correction gradient
void p_pcGrad(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		vec3 grad = nvec3();

		for (int j = 0; j < 4; ++j) {
			vec3 s = vec3Outwards(&o->volumes[i].centroid,
								  &o->volumes[i].faces[j]->centroid,
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

		p_facePI(f);

		f->p = f->pi;
	}

	p_pGrad(o);

	for (int k = 0; k < GRADIT; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue;
			}

			real wf = getGWF(f);
			
			vec3 gc0 = vec3Copy(&f->thisVol[0]->pGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pGrad);

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rc0 = vec3Copy(&f->thisVol[0]->centroid);
			vec3 rc1 = vec3Copy(&f->thisVol[1]->centroid);

			vec3Sub(&rc0, &rf); // rc0 - rf
			vec3Sub(&rc1, &rf);

			real c0 = -wf * (vec3Dot(&gc0, &rc0)); // wf gc0 . (rf - rc0)
			real c1 = -(1.0 - wf) * (vec3Dot(&gc1, &rc1));

			f->p = f->pi + c0 + c1;
		}

		p_pGrad(o);
	}
}

// volume velocity gradient
void p_vGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		f->vin = vec3Copy(&f->vi);
		f->vn = vec3Copy(&f->v);

		p_faceVI(f);

		f->v = vec3Copy(&f->vi);
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

			real wf = getGWF(f);
			
			mat gc0 = matCopy(&f->thisVol[0]->vGrad);
			mat gc1 = matAdd(&gc0, &f->thisVol[1]->vGrad);

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rc0 = vec3Copy(&f->thisVol[0]->centroid);
			vec3 rc1 = vec3Copy(&f->thisVol[1]->centroid);

			vec3Sub(&rc0, &rf); // rc0 - rf
			vec3Sub(&rc1, &rf);

			mat rc0m = matVec3(&rc0);
			mat rc1m = matVec3(&rc1);

			mat gr0 = matMul(&gc0, &rc0m);
			mat gr1 = matMul(&gc1, &rc1m);

			vec3 cv0 = vec3Mat(&gr0);
			vec3 cv1 = vec3Mat(&gr1);

			vec3Mul(&cv0, -wf);
			vec3Mul(&cv1, -(1.0 - wf));

			f->v = vec3Copy(&f->vi);
			vec3Add(&f->v, &cv0);
			vec3Add(&f->v, &cv1);

			matDestroy(&gc0);
			matDestroy(&gc1);
			matDestroy(&rc0m);
			matDestroy(&rc1m);
			matDestroy(&gr0);
			matDestroy(&gr1);
		}

		p_vGrad(o);
	}
}

// pressure correction gradient
void p_pcGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_facePCI(f);

		f->pc = f->pci;
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

			real wf = getGWF(f);

			vec3 gc0 = vec3Copy(&f->thisVol[0]->pcGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pcGrad);

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rc0 = vec3Copy(&f->thisVol[0]->centroid);
			vec3 rc1 = vec3Copy(&f->thisVol[1]->centroid);

			vec3Sub(&rc0, &rf); // rc0 - rf
			vec3Sub(&rc1, &rf);

			real c0 = -wf * (vec3Dot(&gc0, &rc0)); // wf gc0 . (rf - rc0)
			real c1 = -(1.0 - wf) * (vec3Dot(&gc1, &rc1));

			f->pc = f->pci + c0 + c1;
		}

		p_pcGrad(o);
	}
}

