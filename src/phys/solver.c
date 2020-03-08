//
// Created by rak on 2/25/20.
//

#include "solver.h"

// calculate face mass flow rate for volume
float p_getMrate(struct Volume *v, struct Face *f, float rho)
{
	vec3 s = vec3Outwards(&v->centroid,
						  &f->centroid,
						  &f->surface);

	return rho * vec3Dot(&f->v, &s);
}

// volume upwind from face
struct Volume *getUpwindVol(struct Face *f)
{
	vec3 vs = vec3Outwards(&f->centroid,
						   &f->thisVol[0]->centroid,
						   &f->normal);

	float maxd = -vec3Dot(&vs, &f->thisVol[0]->v);

	if (f->vNum < 2)
		return f->thisVol[0];

	vs = vec3Outwards(&f->centroid,
					  &f->thisVol[1]->centroid,
					  &f->normal);

	float maxf = -vec3Dot(&vs, &f->thisVol[1]->v);

	if (maxf > maxd)
		return f->thisVol[1];

	return f->thisVol[0];
}

// face upwind from volume
struct Face *getUpwindFace(struct Volume *v)
{
	struct Face *maxf = v->faces[0];

	vec3 fs = vec3Outwards(&v->centroid,
						   &v->faces[0]->centroid,
						   &v->faces[0]->normal);

	float maxd = -vec3Dot(&fs, &v->v);

	for (int i = 0; i < 4; ++i) {
		fs = vec3Outwards(&v->centroid,
						  &v->faces[i]->centroid,
						  &v->faces[i]->normal);

		float dot = -vec3Dot(&fs, &v->v);

		if (dot > maxd) {
			maxd = dot;
			maxf = v->faces[i];
		}
	}

	return maxf;
}

void p_getVX(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].v.x = o->vx.m[i][0];
	}
}

void p_getVY(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].v.y = o->vy.m[i][0];
	}
}

void p_getVZ(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].v.z = o->vz.m[i][0];
	}
}

// get pressure from matrix
void p_getP(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].pc = o->vp.m[i][0];
	}
}

// construct pressure equation matrix
void p_constructPMat(Obj *o)
{
	o->a = Mat(1.0f, o->volNum, o->volNum);
	o->b = Mat(0.0f, o->volNum, 1);

	o->a.rmin = (int *) malloc(sizeof(int) * o->volNum);
	o->a.rmax = (int *) malloc(sizeof(int) * o->volNum);

	for (int i = 0; i < o->volNum; ++i) {
		o->a.m[i][i] = o->volumes[i].pa;

		o->a.rmin[i] = i;
		o->a.rmax[i] = i;
	}

	for (int i = 0; i < o->volNum; ++i) {
		for (int j = 0; j < o->volumes[i].neiNum; ++j) {
			int n = o->volumes[i].neighbours[j]->index;

			if (n > o->volNum)
				continue;

			struct Face *connecting = p_connectingFace(&o->volumes[i], &o->volumes[n]);

			if (n < o->a.rmin[i])
				o->a.rmin[i] = n;

			if (n > o->a.rmax[i])
				o->a.rmax[i] = n;

			o->a.m[i][n] = -o->fluid.rho * connecting->df;
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].pb;
	}
}

void p_constructVMatX(Obj *o)
{
	o->a = Mat(1.0f, o->volNum, o->volNum);
	o->b = Mat(0.0f, o->volNum, 1);

	o->a.rmin = (int *) malloc(sizeof(int) * o->volNum);
	o->a.rmax = (int *) malloc(sizeof(int) * o->volNum);

	for (int i = 0; i < o->volNum; ++i) {
		o->a.m[i][i] = o->volumes[i].va.x;

		o->a.rmin[i] = i;
		o->a.rmax[i] = i;
	}

	for (int i = 0; i < o->volNum; ++i) {
		for (int j = 0; j < o->volumes[i].neiNum; ++j) {
			int n = o->volumes[i].neighbours[j]->index;

			if (n > o->volNum)
				continue;

			struct Face *connecting = p_connectingFace(&o->volumes[i], &o->volumes[n]);

			if (n < o->a.rmin[i])
				o->a.rmin[i] = n;

			if (n > o->a.rmax[i])
				o->a.rmax[i] = n;

			float mr = fmax(p_getMrate(&o->volumes[n], connecting, o->fluid.rho), 0.0f);

			o->a.m[i][n] = -(connecting->flux.x + mr);
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].vb.x;
	}
}

void p_constructVMatY(Obj *o)
{
	o->a = Mat(1.0f, o->volNum, o->volNum);
	o->b = Mat(0.0f, o->volNum, 1);

	o->a.rmin = (int *) malloc(sizeof(int) * o->volNum);
	o->a.rmax = (int *) malloc(sizeof(int) * o->volNum);

	for (int i = 0; i < o->volNum; ++i) {
		o->a.m[i][i] = o->volumes[i].va.y;

		o->a.rmin[i] = i;
		o->a.rmax[i] = i;
	}

	for (int i = 0; i < o->volNum; ++i) {
		for (int j = 0; j < o->volumes[i].neiNum; ++j) {
			int n = o->volumes[i].neighbours[j]->index;

			if (n > o->volNum)
				continue;

			struct Face *connecting = p_connectingFace(&o->volumes[i], &o->volumes[n]);

			if (n < o->a.rmin[i])
				o->a.rmin[i] = n;

			if (n > o->a.rmax[i])
				o->a.rmax[i] = n;

			float mr = fmax(p_getMrate(&o->volumes[n], connecting, o->fluid.rho), 0.0f);

			o->a.m[i][n] = -(connecting->flux.y + mr);
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].vb.y;
	}
}

void p_constructVMatZ(Obj *o)
{
	o->a = Mat(1.0f, o->volNum, o->volNum);
	o->b = Mat(0.0f, o->volNum, 1);

	o->a.rmin = (int *) malloc(sizeof(int) * o->volNum);
	o->a.rmax = (int *) malloc(sizeof(int) * o->volNum);

	for (int i = 0; i < o->volNum; ++i) {
		o->a.m[i][i] = o->volumes[i].va.z;

		o->a.rmin[i] = i;
		o->a.rmax[i] = i;
	}

	for (int i = 0; i < o->volNum; ++i) {
		for (int j = 0; j < o->volumes[i].neiNum; ++j) {
			int n = o->volumes[i].neighbours[j]->index;

			if (n > o->volNum)
				continue;

			struct Face *connecting = p_connectingFace(&o->volumes[i], &o->volumes[n]);

			if (n < o->a.rmin[i])
				o->a.rmin[i] = n;

			if (n > o->a.rmax[i])
				o->a.rmax[i] = n;

			float mr = fmax(p_getMrate(&o->volumes[n], connecting, o->fluid.rho), 0.0f);

			o->a.m[i][n] = -(connecting->flux.z + mr);
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].vb.z;
	}
}

void p_faceD(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2 || !f->boundary) {
		f->d = f->thisVol[0]->d;
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);

	float g0 = vec3Len(&d0) / vec3Len(&f->volDist);

	vec3 pd0 = vec3Copy(&f->thisVol[0]->d);
	vec3Mul(&pd0, g0);

	vec3 pd1 = vec3Copy(&f->thisVol[1]->d);
	vec3Mul(&pd1, (1.0f - g0));

	f->d = vec3Copy(&pd0);
	vec3Add(&f->d, &pd1);
}

void p_D(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].d = Vec3(o->volumes[i].vol / o->volumes[i].va.x,
							   o->volumes[i].vol / o->volumes[i].va.y,
							   o->volumes[i].vol / o->volumes[i].va.z);
	}
	for (int i = 0; i < o->faceNum; ++i) {
		p_faceD(&o->faces[i]);
	}

	for (int i = 0; i < o->faceNum; ++i) {
		float d = 0.0f;

		vec3 dist = vec3Copy(&o->faces[i].volDist);

		d = (dist.x * o->faces[i].d.x * o->faces[i].surface.x +
			 dist.y * o->faces[i].d.y * o->faces[i].surface.y +
			 dist.z * o->faces[i].d.z * o->faces[i].surface.z);

		d /= (dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);

		o->faces[i].df = fabs(d);
	}
}

// calculate coefficients for pressure equation
void p_computePCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].pa = 0.0f;
		for (int j = 0; j < 4; ++j) {
			o->volumes[i].pa += o->fluid.rho * o->volumes[i].faces[j]->df;
		}

		o->volumes[i].pb = 0.0f;
		for (int j = 0; j < 4; ++j) {
			o->volumes[i].pb -= p_getMrate(&o->volumes[i], o->volumes[i].faces[j], o->fluid.rho);
		}
	}
}

// calculate coefficients for momentum equation
void p_computeVCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].va.x = o->volumes[i].flux;
		o->volumes[i].va.y = o->volumes[i].flux;
		o->volumes[i].va.z = o->volumes[i].flux;

		for (int j = 0; j < 4; ++j) {
			float mr = fmax(p_getMrate(&o->volumes[i], o->volumes[i].faces[j], o->fluid.rho), 0.0f);

			o->volumes[i].va.x += o->volumes[i].faces[j]->flux.x + mr;
			o->volumes[i].va.y += o->volumes[i].faces[j]->flux.y + mr;
			o->volumes[i].va.z += o->volumes[i].faces[j]->flux.z + mr;
		}
		o->volumes[i].va.x /= o->vRelax;
		o->volumes[i].va.y /= o->vRelax;
		o->volumes[i].va.z /= o->vRelax;

		o->volumes[i].vb = nvec3();
		vec3Sub(&o->volumes[i].vb, &o->volumes[i].vFlux);

		vec3 b0 = vec3Copy(&o->volumes[i].pGrad);
		vec3Mul(&b0, o->volumes[i].vol);

		vec3 b1 = nvec3();
		vec3 b2 = nvec3();

		vec3 br = vec3Copy(&o->volumes[i].v);
		br.x *= (1.0f - o->vRelax) * o->volumes[i].va.x;
		br.y *= (1.0f - o->vRelax) * o->volumes[i].va.y;
		br.z *= (1.0f - o->vRelax) * o->volumes[i].va.z;

		for (int j = 0; j < 4; ++j) {
			vec3 cf = vec3Copy(&o->volumes[i].faces[j]->conFlux);

			float mr = p_getMrate(&o->volumes[i], o->volumes[i].faces[j], o->fluid.rho);

			vec3Mul(&cf, (mr));

			vec3Add(&b1, &o->volumes[i].faces[j]->vFlux);
			vec3Add(&b1, &cf);

			mat g = matCopy(&o->volumes[i].faces[j]->vGrad);
			mat tm = matTranspose(&g);
			mat v0 = matVec3(&o->volumes[i].faces[j]->surface);
			mat v1 = matMul(&tm, &v0);
			vec3 v2 = vec3Mat(&v1);
			vec3Mul(&v2, o->fluid.mu);

			vec3Add(&b2, &v2);

			matDestroy(&g);
			matDestroy(&tm);
			matDestroy(&v0);
			matDestroy(&v1);
		}

		vec3Sub(&o->volumes[i].vb, &b0);
		vec3Sub(&o->volumes[i].vb, &b1);
		vec3Add(&o->volumes[i].vb, &b2);

		vec3Add(&o->volumes[i].vb, &br);
	}
}

// compute face fluxes
void p_computeFaceFs(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].conFlux = nvec3();
	}

	for (int i = 0; i < o->volNum; ++i) {
		struct Face *uf = getUpwindFace(&o->volumes[i]);

		uf->conFlux = vec3Copy(&o->volumes[i].v);
		vec3Sub(&uf->conFlux, &uf->v);
	}

	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum < 2) {
			continue;
		} else {
			o->faces[i].flux.x = o->fluid.mu * o->faces[i].df;
			o->faces[i].flux.y = o->fluid.mu * o->faces[i].df;
			o->faces[i].flux.z = o->fluid.mu * o->faces[i].df;
		}

		mat t = matVec3(&o->faces[i].surfaceT);

		mat v0 = matMul(&o->faces[i].vGrad, &t);
		o->faces[i].vFlux = vec3Mat(&v0);
		vec3Mul(&o->faces[i].vFlux, -o->fluid.mu);

		matDestroy(&t);
		matDestroy(&v0);
	}
}

// compute volume fluxes
void p_computeVolFs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].flux = (o->fluid.rho * o->volumes[i].vol) / o->dt;

		o->volumes[i].vFlux = vec3Copy(&o->volumes[i].v);
		vec3Mul(&o->volumes[i].vFlux, -(o->fluid.rho * o->volumes[i].vol) / o->dt);

		vec3 s = vec3Copy(&o->fluid.f);
		vec3Add(&s, &o->volumes[i].s);

		vec3Mul(&s, o->volumes[i].vol);

		vec3Sub(&o->volumes[i].vFlux, &s);
	}
}

// minimum correction decomposition of surfaces
void p_decomposeSurfs(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1) {
			o->faces[i].surfaceT = nvec3();
			o->faces[i].surfaceE = vec3Copy(&o->faces[i].surface);

			vec3 d = vec3Copy(&o->faces[i].thisVol[0]->centroid);
			vec3Sub(&d, &o->faces[i].centroid);

			o->faces[i].volDist = vec3Copy(&d);
		}
	}
	for (int i = 0; i < o->volNum; ++i) {
		for (int jj = 0; jj < o->volumes[i].neiNum; ++jj) {
			int j = o->volumes[i].neighbours[jj]->index;
			struct Face *c = p_connectingFace(&o->volumes[i], &o->volumes[j]);
			if (c == NULL)
				continue;

			vec3 e = vec3Copy(&o->volumes[i].centroid);
			vec3Sub(&e, &o->volumes[j].centroid);
			c->volDist = vec3Copy(&e);

			vec3Normalize(&e);

			vec3 s = vec3Outwards(&o->volumes[i].centroid,
								  &o->volumes[j].centroid,
								  &c->surface);

			float vd = vec3Dot(&e, &s);

			c->surfaceE = vec3Copy(&e);
			vec3Mul(&c->surfaceE, vd);

			c->surfaceT = vec3Copy(&c->surface);
			vec3Sub(&c->surfaceT, &c->surfaceE);
		}
	}
}

// interpolate face velocity from volumes
void p_faceVI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2 || f->boundary) {
		f->vi = f->thisVol[0]->v;
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);

	float g0 = vec3Len(&d0) / vec3Len(&f->volDist);

	vec3 vel0 = vec3Copy(&f->thisVol[0]->v);
	vec3Mul(&vel0, g0);

	vec3 vel1 = vec3Copy(&f->thisVol[1]->v);
	vec3Mul(&vel1, (1.0f - g0));

	f->vi = vec3Copy(&vel0);
	vec3Add(&f->vi, &vel1);
}

// interpolate face pressure from volumes
void p_facePI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2 || f->boundary) {
		f->pi = f->thisVol[0]->p;
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);

	float g0 = vec3Len(&d0) / vec3Len(&f->volDist);

	float pre0 = f->thisVol[0]->p * g0;

	float pre1 = f->thisVol[1]->p * (1.0f - g0);

	f->pi = pre0 + pre1;
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

	if (f->boundary) {
		vec3Sub(&vc, &f->thisVol[0]->pGrad);
	} else {
		vec3Sub(&vc, &f->pGradI);
	}

	vc.x *= f->d.x;
	vc.y *= f->d.y;
	vc.z *= f->d.z;

	vec3Add(&f->v, &vc);
}

void p_faceV(struct Face *f)
{
	p_faceVI(f);

	f->v = vec3Copy(&f->vi);

	p_faceVRC(f);
}

// interpolated face pressure gradient
void p_pFaceGradI(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1) {
			o->faces[i].pGradI = vec3Copy(&o->faces[i].thisVol[0]->pGrad);
		} else if (o->faces[i].vNum == 2) {
			vec3 g0 = vec3Copy(&o->faces[i].thisVol[0]->pGrad);
			vec3 g1 = vec3Copy(&o->faces[i].thisVol[1]->pGrad);

			vec3 d0 = vec3Copy(&o->faces[i].thisVol[1]->centroid);
			vec3Sub(&d0, &o->faces[i].centroid);
			float wf = vec3Len(&d0) / vec3Len(&o->faces[i].volDist);

			vec3Mul(&g0, wf);
			vec3Mul(&g1, 1.0f - wf);
			vec3Add(&g0, &g1);

			o->faces[i].pGradI = vec3Copy(&g0);
		}
	}
}

// interpolated face velocity gradient
void p_vFaceGradI(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1) {
			matDestroy(&o->faces[i].vGradI);
			o->faces[i].vGradI = matCopy(&o->faces[i].thisVol[0]->vGrad);
		} else if (o->faces[i].vNum == 2) {
			matDestroy(&o->faces[i].vGradI);
			mat g0 = matCopy(&o->faces[i].thisVol[0]->vGrad);
			mat g1 = matCopy(&o->faces[i].thisVol[1]->vGrad);

			vec3 d0 = vec3Copy(&o->faces[i].thisVol[1]->centroid);
			vec3Sub(&d0, &o->faces[i].centroid);

			float wf = vec3Len(&d0) / vec3Len(&o->faces[i].volDist);

			mat s0 = Mat(wf, 3, 3);
			mat s1 = Mat(1.0f - wf, 3, 3);

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

		float c = (o->faces[i].thisVol[0]->p - o->faces[i].thisVol[1]->p) / vec3Len(&o->faces[i].volDist);
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

		vec3 v0 = vec3Copy(&o->faces[i].thisVol[0]->v);
		vec3 v1 = vec3Copy(&o->faces[i].thisVol[1]->v);

		vec3Sub(&v0, &v1);
		vec3Mul(&v0, 1.0f / vec3Len(&e));
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
		*grad = Mat(0.0f, 3, 3);

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

		mat s = Mat(o->volumes[i].vol, 3, 3);
		mat a = matMul(grad, &s);
		matDestroy(&s);
		matDestroy(grad);

		*grad = matCopy(&a);
		matDestroy(&a);
	}
}

// interpolate face pressure correction from volumes
void p_pcFaceI(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2 || f->boundary) {
		f->pc = f->thisVol[0]->pc;
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);

	float g0 = vec3Len(&d0) / vec3Len(&f->volDist);

	float pre0 = f->thisVol[0]->pc * g0;

	float pre1 = f->thisVol[1]->pc * (1.0f - g0);

	f->pc = pre0 + pre1;
}

// volume pressure correction gradient
void p_pcGrad(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		p_pcFaceI(&o->faces[i]);
	}

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

