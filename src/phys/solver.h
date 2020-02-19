#ifndef SOLVER_H
#define SOLVER_H

#include "volume.h"
<<<<<<< HEAD
#include "sys.h"

void p_getV(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].v = Vec3(
				o->v.m[i * 3 + 0][0],
				o->v.m[i * 3 + 1][0],
				o->v.m[i * 3 + 2][0]
		);
	}
}

void p_getP(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].p = o->v.m[i][0];
	}
}

void p_constructPMat(Obj *o)
{
	o->a = Mat(1.0f, o->volNum, o->volNum);
	o->b = Mat(0.0f, o->volNum, 1);

	for (int i = 0; i < o->volNum; ++i) {
		float left = o->volumes[i].pa;

		o->a.m[i][i] = left;
	}

	for (int i = 0; i < o->volNum; i += 1) {
		for (int j = 0; j < o->volumes[i].neiNum; ++j) {
			int n = o->volumes[i].neighbours[j]->index;

			if (n > o->volNum)
				continue;

			struct Face *connecting = p_connectingFace(&o->volumes[i], &o->volumes[n]);

			o->a.m[i][n] = connecting->flux;
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		float right = o->volumes[i].pb;

		o->b.m[i][0] = right;
	}
}

void p_computePCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].pa = 0.0f;
		for (int j = 0; j < 4; ++j) {
			o->volumes[i].pa -= o->volumes[i].faces[j]->flux;
		}

		o->volumes[i].pb = 0.0f;
		for (int j = 0; j < 4; ++j) {
			o->volumes[i].pb -= vec3Len(&o->volumes[i].faces[j]->vFlux); // length?
		}
	}
}

void p_constructVMat(Obj *o)
{
	o->a = Mat(1.0f, o->volNum * 3, o->volNum * 3);
	o->b = Mat(0.0f, o->volNum * 3, 1);

	for (int i = 0; i < o->volNum; i += 1) {
		float left = o->volumes[i].va;

		o->a.m[i + o->volNum * 0][i + o->volNum * 0] = left;
		o->a.m[i + o->volNum * 1][i + o->volNum * 1] = left;
		o->a.m[i + o->volNum * 2][i + o->volNum * 2] = left;
	}

	for (int i = 0; i < o->volNum; i += 1) {
		for (int j = 0; j < o->volumes[i].neiNum; ++j) {
			int n = o->volumes[i].neighbours[j]->index;

			if (n > o->volNum)
				continue;

			struct Face *connecting = p_connectingFace(&o->volumes[i], &o->volumes[n]);

			o->a.m[i + o->volNum * 0][n + o->volNum * 0] = -connecting->flux;
			o->a.m[i + o->volNum * 1][n + o->volNum * 1] = -connecting->flux;
			o->a.m[i + o->volNum * 2][n + o->volNum * 2] = -connecting->flux;
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		vec3 right = vec3Copy(&o->volumes[i].vb);

		o->b.m[i + o->volNum * 0][0] = right.x;
		o->b.m[i + o->volNum * 1][0] = right.y;
		o->b.m[i + o->volNum * 2][0] = right.z;
	}
}

void p_computeVCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].va = o->volumes[i].flux;

		for (int j = 0; j < 4; ++j) {
			o->volumes[i].va += o->volumes[i].faces[j]->flux;
		}

		o->volumes[i].vb = nvec3();
		vec3Sub(&o->volumes[i].vb, &o->volumes->vFlux);
=======

void p_constructMats(Obj *o)
{
	o->a = Mat(1.0f, o->volNum * 3, o->volNum * 3);
	o->b = Mat(1.0f, o->volNum * 3, 1);
	o->v = Mat(1.0f, o->volNum * 3, 1);
}

void p_computeCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].a = o->volumes[i].flux;

		for (int j = 0; j < 4; ++j) {
			o->volumes[i].a += o->volumes[i].faces[j]->flux;
		}

		o->volumes[i].b = nvec3();
		vec3Sub(&o->volumes[i].b, &o->volumes->vFlux);
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c

		vec3 b0 = vec3Copy(&o->volumes[i].pGrad);
		vec3Mul(&b0, o->volumes[i].vol);

		vec3 b1 = nvec3();
		vec3 b2 = nvec3();

		for (int j = 0; j < 4; ++j) {
			vec3Add(&b1, &o->volumes[i].faces[j]->vFlux);

			mat g = matCopy(&o->volumes[i].faces[j]->vGrad);
<<<<<<< HEAD
			mat tm = matTranspose(&g);
			mat v0 = matVec3(&o->volumes[i].faces[j]->surface);
			mat v1 = matMul(&tm, &v0);
=======
			matTranspose(&g);
			mat v0 = matVec3(&o->volumes[i].faces[j]->surface);
			mat v1 = matMul(&g, &v0);
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
			vec3 v2 = vec3Mat(&v1);
			vec3Mul(&v2, o->fluid.mu);

			vec3Add(&b2, &v2);

			matDestroy(&g);
<<<<<<< HEAD
			matDestroy(&tm);
=======
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
			matDestroy(&v0);
			matDestroy(&v1);
		}

<<<<<<< HEAD
		vec3Sub(&o->volumes[i].vb, &b0);
		vec3Sub(&o->volumes[i].vb, &b1);
		vec3Add(&o->volumes[i].vb, &b2);
=======
		vec3Sub(&o->volumes[i].b, &b0);
		vec3Sub(&o->volumes[i].b, &b1);
		vec3Add(&o->volumes[i].b, &b2);

		vec3Print(&o->volumes[i].b);
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
	}
}

void p_computeFaceFs(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
<<<<<<< HEAD
		if (o->faces[i].vNum < 2) {
			continue;
		}
		else {
			o->faces[i].flux = o->faces[i].mRate;
			o->faces[i].flux += o->fluid.mu * (vec3Len(&o->faces[i].surfaceE) / vec3Len(&o->faces[i].volDist));
		}
=======
		if (vec3Len(&o->faces[i].volDist) == 0.0f)
			continue;

		o->faces[i].flux = vec3Len(&o->faces[i].mFlux);
		o->faces[i].flux += o->fluid.mu * (vec3Len(&o->faces[i].surfaceE) / vec3Len(&o->faces[i].volDist));
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c

		mat t = matVec3(&o->faces[i].surfaceT);

		mat v0 = matMul(&t, &o->faces[i].vGrad);
		o->faces[i].vFlux = vec3Mat(&v0);
		vec3Mul(&o->faces[i].vFlux, -o->fluid.mu);

		//vec3 v1 =

		matDestroy(&t);
		matDestroy(&v0);
	}
}

void p_computeVolFs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].flux = (o->fluid.rho * o->volumes[i].vol) / o->dt;

		o->volumes[i].vFlux = vec3Copy(&o->volumes[i].v);
		vec3Mul(&o->volumes[i].vFlux, -(o->fluid.rho * o->volumes[i].vol) / o->dt);

		vec3 s = vec3Copy(&o->f);
		vec3Mul(&s, o->volumes[i].vol);

		vec3Sub(&o->volumes[i].vFlux, &s);
	}
}

// minimum correction
void p_decomposeSurfs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
<<<<<<< HEAD
		for (int jj = 0; jj < o->volumes[i].neiNum; ++jj) {
			int j = o->volumes[i].neighbours[jj]->index;
=======
		for (int j = i + 1; j < o->volNum; ++j) {
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
			struct Face *c = p_connectingFace(&o->volumes[i], &o->volumes[j]);
			if (c == NULL)
				continue;

<<<<<<< HEAD
			vec3 e = vec3Copy(&o->volumes[i].centroid);
=======
			vec3 e = o->volumes[i].centroid;
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
			vec3Sub(&e, &o->volumes[j].centroid);
			c->volDist = vec3Copy(&e);

			vec3Normalize(&e);

<<<<<<< HEAD
			vec3 s = vec3Outwards(&o->volumes[i].centroid,
								  &o->volumes[j].centroid,
								  &c->surface);

			float vd = vec3Dot(&e, &s);

			c->surfaceE = vec3Copy(&e);
			vec3Mul(&c->surfaceE, vd);

			c->surfaceT = vec3Copy(&c->surface);
			vec3Sub(&c->surfaceT, &c->surfaceE);
=======
			c->surfaceE = vec3Copy(&e);
			vec3Mul(&c->surfaceE, vec3Dot(&e, &c->surface));

			c->surfaceT = vec3Copy(&c->surface);
			vec3Sub(&c->surfaceT, &c->surface);
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
		}
	}
}

void p_faceV(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->v = f->thisVol[0]->v;
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);
<<<<<<< HEAD

	vec3Sub(&d0, &f->centroid);
=======
	//vec3 d1 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);
	//vec3Sub(&d1, &f->thisVol[0]->centroid);
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c

	float g0 = vec3Len(&d0) / vec3Len(&f->volDist);

	vec3 vel0 = vec3Copy(&f->thisVol[0]->v);
	vec3Mul(&vel0, g0);

	vec3 vel1 = vec3Copy(&f->thisVol[1]->v);
	vec3Mul(&vel1, (1.0f - g0));

	f->v = vec3Copy(&vel0);
	vec3Add(&f->v, &vel1);
}

void p_faceP(struct Face *f)
{
	if (f->vNum == 0)
		return;

	if (f->vNum < 2) {
		f->p = f->thisVol[0]->p;
		return;
	}

	vec3 d0 = vec3Copy(&f->thisVol[1]->centroid);
<<<<<<< HEAD

	vec3Sub(&d0, &f->centroid);
=======
	//vec3 d1 = vec3Copy(&f->thisVol[1]->centroid);

	vec3Sub(&d0, &f->centroid);
	//vec3Sub(&d1, &f->thisVol[0]->centroid);
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c

	float g0 = vec3Len(&d0) / vec3Len(&f->volDist);

	float pre0 = f->thisVol[0]->p * g0;

	float pre1 = f->thisVol[1]->p * (1.0f - g0);

	f->p = pre0 + pre1;
}

<<<<<<< HEAD
void p_vFaceGrad(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1) {
			matDestroy(&o->faces[i].vGrad);
			o->faces[i].vGrad = matCopy(&o->faces[i].thisVol[0]->vGrad);
		}
		else if (o->faces[i].vNum == 2) {
			matDestroy(&o->faces[i].vGrad);
			mat g0 = matCopy(&o->faces[i].thisVol[0]->vGrad);
			mat g1 = matCopy(&o->faces[i].thisVol[1]->vGrad);

			mat s = Mat(0.5f, 3, 3);

			mat g = matAdd(&g0, &g1);
			o->faces[i].vGrad = matMul(&g, &s);

			matDestroy(&g0);
			matDestroy(&g1);
			matDestroy(&s);
			matDestroy(&g);
		}
	}
}

=======
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
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

void p_vGrad(Obj *o) // check
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
<<<<<<< HEAD

		*grad = matCopy(&a);
		matDestroy(&a);

		/*matPrint(grad);
		printf("\n");*/
=======
		*grad = matCopy(&a);
		matDestroy(&a);

		//matPrint(grad);
		//printf("\n");
>>>>>>> b025922178a273222b9191905a44e8161a0eea1c
	}
}

#endif
