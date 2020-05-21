//
// Created by rak on 4/10/20.
//

#include "gradient.h"

#define HALF // halfway approximation
#define GRADIT 4 // gradient iterations
#define RCTRANS // rhie-chow transient term

// calculate geometric weighting factor
real getGWF(struct Face *f)
{
	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);

	return vec3Len(&d0) / vec3Len(&f->volDist);
}

// calculate geometric weighting factor
real getGWFp(struct Face *f)
{
#ifdef HALF
	return 0.5;
#endif

	return getGWF(f);

	vec3 e = vec3Copy(&f->volDist);
	vec3Normalize(&e);

	vec3 fc = vec3Copy(&e);
	real fd = vec3Dot(&f->centroid, &f->normal) / vec3Dot(&e, &f->normal);
	vec3Mul(&fc, fd);

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &fc);

	return vec3Len(&d0) / vec3Len(&f->volDist);
}

void p_faceD(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->d = f->thisVol[0]->d;
		return;
	}

	real g0 = getGWF(f);

	vec3 pd0 = vec3Copy(&f->thisVol[0]->d);
	vec3Mul(&pd0, g0);

	vec3 pd1 = vec3Copy(&f->thisVol[1]->d);
	vec3Mul(&pd1, (1.0 - g0));

	f->d = vec3Copy(&pd0);
	vec3Add(&f->d, &pd1);
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

	real g0 = getGWFp(f);

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
		f->pi = f->thisVol[0]->p;
		return;
	}

	real g0 = getGWFp(f);

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

	real g0 = getGWFp(f);

	real pre0 = f->thisVol[0]->pc * g0;

	real pre1 = f->thisVol[1]->pc * (1.0 - g0);

	f->pci = pre0 + pre1;
}

void p_facePC(struct Face *f)
{
	p_facePCI(f);

	f->pc = f->pci;
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

	real pc = (vec3Dot(&vol->pGrad, &s) - vec3Dot(&f->pGrad, &t)) / f->df; // d_c == e / d_cb? or d?
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

	f->p = f->pi;

	switch (f->boundary) {
		case 4:
			f->p = f->constantP;
			return;
	}
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

	vc = vec3Copy(&f->vn); // under-relaxation
	vec3Sub(&vc, &f->vin);
	vec3Mul(&vc, 1.0 - urf);

	vec3Add(&f->v, &vc);

#ifndef RCTRANS
	return;
#endif

	vc = vec3Copy(&f->vtn); // transient (first-order euler only)
	vec3Sub(&vc, &f->vitn);

	if (f->dtn.x == 0.0 || f->dtn.y == 0.0 || f->dtn.z == 0.0)
		return;

	vc.x *= f->d.x / f->dtn.x;
	vc.y *= f->d.y / f->dtn.y;
	vc.z *= f->d.z / f->dtn.z;

	vec3Add(&f->v, &vc);
}

void fv(struct Face *f)
{
	p_faceVI(f);

	f->v = vec3Copy(&f->vi);

	vec3 cv;
	switch (f->boundary) {
		case 1:
			//f->v = nvec3();
			return;
		case 3:
			cv = vec3Copy(&f->normal);
			vec3Mul(&cv, f->constantV);
			f->v = vec3Copy(&cv);
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
			matDestroy(&o->faces[i].vGradI);
			o->faces[i].vGradI = matCopy(&o->faces[i].thisVol[0]->vGrad);
		} else {
			matDestroy(&o->faces[i].vGradI);
			mat g0 = matCopy(&o->faces[i].thisVol[0]->vGrad);
			mat g1 = matCopy(&o->faces[i].thisVol[1]->vGrad);

			real wf = getGWF(&o->faces[i]);

			mat s0 = Mat(wf, 3, 3);
			mat s1 = Mat(1.0 - wf, 3, 3);

			mat gg0 = matMul(&g0, &s0);
			mat gg1 = matMul(&g1, &s1);

			o->faces[i].vGradI = matAdd(&gg0, &gg1);

			/*printf("=\n");
			matPrint(&g0);
			printf("-\n");
			matPrint(&g1);
			printf("-\n");
			matPrint(&o->faces[i].vGradI);
			printf(".\n");*/

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
		matDestroy(&o->faces[i].vGrad);

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
		mat vm = matVec3(&v0);

		mat em = matVec3(&e);
		mat emt = matTranspose(&em);
		mat vg1 = matMul(&o->faces[i].vGradI, &em);

		mat vg2 = matSub(&vm, &vg1);
		mat vg3 = matMul(&vg2, &emt);

		o->faces[i].vGrad = matAdd(&o->faces[i].vGradI, &vg3);

		matDestroy(&vm);
		matDestroy(&em);
		matDestroy(&emt);
		matDestroy(&vg1);
		matDestroy(&vg2);
		matDestroy(&vg3);
	}
}

// volume pressure gradient (Green-Gauss)
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

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rc0 = vec3Copy(&rf);
			vec3 rc1 = vec3Copy(&rf);

			vec3Sub(&rc0, &f->thisVol[0]->centroid); // rf - rc
			vec3Sub(&rc1, &f->thisVol[1]->centroid);

			real c0 = wf * (vec3Dot(&gc0, &rc0)); // wf gc0 . (rf - rc0)
			real c1 = (1.0 - wf) * (vec3Dot(&gc1, &rc1));

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

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rc0 = vec3Copy(&rf);
			vec3 rc1 = vec3Copy(&rf);

			vec3Sub(&rc0, &f->thisVol[0]->centroid); // rf - rc
			vec3Sub(&rc1, &f->thisVol[1]->centroid);

			mat rc0m = matVec3(&rc0);
			mat rc1m = matVec3(&rc1);

			mat gr0 = matDot(&f->thisVol[0]->vGrad, &rc0m);
			mat gr1 = matDot(&f->thisVol[1]->vGrad, &rc1m);

			vec3 cv0 = vec3Mat(&gr0);
			vec3 cv1 = vec3Mat(&gr1);

			vec3Mul(&cv0, wf);
			vec3Mul(&cv1, (1.0 - wf));

			f->v = vec3Copy(&f->vi);
			vec3Add(&f->v, &cv0);
			vec3Add(&f->v, &cv1);

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

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rc0 = vec3Copy(&rf);
			vec3 rc1 = vec3Copy(&rf);

			vec3Sub(&rc0, &f->thisVol[0]->centroid); // rf - rc
			vec3Sub(&rc1, &f->thisVol[1]->centroid);

			real c0 = wf * (vec3Dot(&gc0, &rc0)); // wf gc0 . (rf - rc0)
			real c1 = (1.0 - wf) * (vec3Dot(&gc1, &rc1));

			f->pc = f->pci + c0 + c1;
		}

		p_pcGrad(o);
	}
}

// volume pressure gradient halfway
void p_pGradsh(Obj *o)
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

			vec3 gc0 = vec3Copy(&f->thisVol[0]->pGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pGrad);

			vec3Add(&gc0, &gc1);

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rcf = vec3Copy(&f->thisVol[0]->centroid);
			vec3Add(&rcf, &f->thisVol[1]->centroid);
			vec3Mul(&rcf, 0.5);

			vec3Sub(&rf, &rcf);

			real c = 0.5 * vec3Dot(&gc0, &rf); // wf gc0 . (rf - rc0)

			f->p = f->pi + c;
		}

		p_pGrad(o);
	}
}

// volume velocity gradient halfway
void p_vGradsh(Obj *o)
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

			mat gc0 = matCopy(&f->thisVol[0]->vGrad);
			mat gc1 = matAdd(&gc0, &f->thisVol[1]->vGrad); // gc0 = vg0 + vg1

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rcf = vec3Copy(&f->thisVol[0]->centroid);
			vec3Add(&rcf, &f->thisVol[1]->centroid);
			vec3Mul(&rcf, 0.5);

			vec3Sub(&rf, &rcf);

			mat rfm = matVec3(&rf);

			mat d = matDot(&gc0, &rfm);
			vec3 c = vec3Mat(&d);
			vec3Mul(&c, 0.5);

			f->v = vec3Copy(&f->vi);
			vec3Add(&f->v, &c);

			matDestroy(&gc0);
			matDestroy(&gc1);
			matDestroy(&rfm);
			matDestroy(&d);
		}

		p_vGrad(o);
	}
}

// pressure correction gradient halfway
void p_pcGradsh(Obj *o)
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

			vec3 gc0 = vec3Copy(&f->thisVol[0]->pcGrad);
			vec3 gc1 = vec3Copy(&f->thisVol[1]->pcGrad);

			vec3Add(&gc0, &gc1);

			vec3 rf = vec3Copy(&f->centroid);

			vec3 rcf = vec3Copy(&f->thisVol[0]->centroid);
			vec3Add(&rcf, &f->thisVol[1]->centroid);
			vec3Mul(&rcf, 0.5);

			vec3Sub(&rf, &rcf);

			real c = 0.5 * vec3Dot(&gc0, &rf); // wf gc0 . (rf - rc0)

			f->pc = f->pci + c;
		}

		p_pcGrad(o);
	}
}
