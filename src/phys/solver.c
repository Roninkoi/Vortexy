#include "solver.h"
#include "util/util.h"

// decomposition method
#define MINDC 0 // minimum correction
#define ORTHODC 0 // orthogonal correction
#define ORDC 1 // over-relaxed correction

//#define CONVSCH // using convection scheme
#define CONVF 1 // first-iteration scheme?
#define CONVT 1 // first time-step scheme?

real p_getMrate(struct Volume *v, struct Face *f, real rho)
{
	vec3 s = vec3Outwards(&v->r,
						  &f->r,
						  &f->surface);

	f->mRate = rho * vec3Dot(&f->v, &s);

	return f->mRate;
}

struct Volume *getUpwindVol(struct Face *f)
{
	vec3 vs = vec3Outwards(&f->thisVol[0]->r,
						   &f->r,
						   &f->normal);

	real maxd = -vec3Dot(&vs, &f->thisVol[0]->v);

	if (f->vNum < 2)
		return f->thisVol[0];

	vs = vec3Outwards(&f->thisVol[1]->r,
					  &f->r,
					  &f->normal);

	real maxf = -vec3Dot(&vs, &f->thisVol[1]->v);

	if (maxf > maxd)
		return f->thisVol[1];

	return f->thisVol[0];
}

struct Face *getUpwindFace(struct Volume *v)
{
	struct Face *maxf = v->faces[0];

	vec3 fs = vec3Outwards(&v->r,
						   &v->faces[0]->r,
						   &v->faces[0]->normal);

	real maxd = -vec3Dot(&fs, &v->v);

	for (int i = 0; i < 4; ++i) {
		fs = vec3Outwards(&v->r,
						  &v->faces[i]->r,
						  &v->faces[i]->normal);

		real dot = -vec3Dot(&fs, &v->v);

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

void p_getP(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].pc = o->vp.m[i][0];
	}
}

void p_computePFaceCoeff(struct Face *connecting, struct Volume *vol, struct Fluid *fluid)
{
	switch (connecting->boundary) {
		case noSlip: // no-slip wall
		case slip: // slip wall
		case vInlet: // velocity inlet
		case pInlet: // pressure inlet
		case vOutlet: // velocity outlet
		case pOutlet: // pressure outlet
			connecting->pa = 0.0;
			return;
	}

	connecting->pa = -fluid->rho * connecting->df;
}

void p_constructPMat(Obj *o)
{
	o->a = Mat(1.0, o->volNum, o->volNum);
	o->b = Matc(0.0, o->volNum);

	o->a.rmin = malloc(sizeof(int) * o->volNum);
	o->a.rmax = malloc(sizeof(int) * o->volNum);

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

			//o->a.m[i][n] = -o->fluid.rho * connecting->df;
			p_computePFaceCoeff(connecting, &o->volumes[i], &o->fluid);

			o->a.m[i][n] = connecting->pa;
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].pb;
	}
}

real p_getVFaceCoeff(struct Face *connecting, struct Volume *vol, struct Fluid *fluid)
{
	real mr = max(-p_getMrate(vol, connecting, fluid->rho), 0.0);

	return -(connecting->flux + mr);
}

void p_computeVFaceCoeff(struct Face *connecting, struct Volume *vol, struct Fluid *fluid)
{
	switch (connecting->boundary) {
		case noSlip: // no-slip wall
		case slip: // slip wall
		case vInlet: // velocity inlet
		case pInlet: // pressure inlet
		case vOutlet: // velocity outlet
		case pOutlet: // pressure outlet
			connecting->va.x = 0.0;
			connecting->va.y = 0.0;
			connecting->va.z = 0.0;
			return;
	}

	real mr = max(-p_getMrate(vol, connecting, fluid->rho), 0.0);

	connecting->va.x = -(connecting->flux + mr);
	connecting->va.y = -(connecting->flux + mr);
	connecting->va.z = -(connecting->flux + mr);
}

void p_constructVMatX(Obj *o)
{
	o->a = Mat(1.0, o->volNum, o->volNum);
	o->b = Matc(0.0, o->volNum);

	o->a.rmin = malloc(sizeof(int) * o->volNum);
	o->a.rmax = malloc(sizeof(int) * o->volNum);

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

			p_computeVFaceCoeff(connecting, &o->volumes[i], &o->fluid);

			o->a.m[i][n] = connecting->va.x;
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].vb.x;
	}
}

void p_constructVMatY(Obj *o)
{
	o->a = Mat(1.0, o->volNum, o->volNum);
	o->b = Matc(0.0, o->volNum);

	o->a.rmin = malloc(sizeof(int) * o->volNum);
	o->a.rmax = malloc(sizeof(int) * o->volNum);

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

			p_computeVFaceCoeff(connecting, &o->volumes[i], &o->fluid);

			o->a.m[i][n] = connecting->va.y;
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].vb.y;
	}
}

void p_constructVMatZ(Obj *o)
{
	o->a = Mat(1.0, o->volNum, o->volNum);
	o->b = Matc(0.0, o->volNum);

	o->a.rmin = malloc(sizeof(int) * o->volNum);
	o->a.rmax = malloc(sizeof(int) * o->volNum);

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

			p_computeVFaceCoeff(connecting, &o->volumes[i], &o->fluid);

			o->a.m[i][n] = connecting->va.z;
		}
	}

	for (int i = 0; i < o->volNum; ++i) {
		o->b.m[i][0] = o->volumes[i].vb.z;
	}
}

void p_computeC(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].c = Vec3(o->volumes[i].vol / o->volumes[i].va.x,
							   o->volumes[i].vol / o->volumes[i].va.y,
							   o->volumes[i].vol / o->volumes[i].va.z);
	}
	for (int i = 0; i < o->faceNum; ++i) {
		p_faceC(&o->faces[i]);
	}

	for (int i = 0; i < o->faceNum; ++i) {
		real d = 0.0;

		vec3 dist = vec3Copy(&o->faces[i].volDist);

#if MINDC
		d = fabs(dist.x * o->faces[i].c.x * o->faces[i].surface.x) +
			fabs(dist.y * o->faces[i].c.y * o->faces[i].surface.y) +
			fabs(dist.z * o->faces[i].c.z * o->faces[i].surface.z); // minimum correction

		d /= (dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);

		o->faces[i].df = d;
#elif ORTHODC
		d = o->faces[i].c.x * o->faces[i].surface.x * o->faces[i].c.x * o->faces[i].surface.x +
			o->faces[i].c.y * o->faces[i].surface.y * o->faces[i].c.y * o->faces[i].surface.y +
			o->faces[i].c.z * o->faces[i].surface.z * o->faces[i].c.z * o->faces[i].surface.z; // orthogonal correction

		d /= (dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);

		o->faces[i].df = sqrt(d); // ortho
#elif ORDC
		d = o->faces[i].c.x * o->faces[i].surface.x * o->faces[i].c.x * o->faces[i].surface.x +
			o->faces[i].c.y * o->faces[i].surface.y * o->faces[i].c.y * o->faces[i].surface.y +
			o->faces[i].c.z * o->faces[i].surface.z * o->faces[i].c.z * o->faces[i].surface.z; // over-relaxed

		d /= (fabs(dist.x * o->faces[i].c.x * o->faces[i].surface.x) +
			  fabs(dist.y * o->faces[i].c.y * o->faces[i].surface.y) +
			  fabs(dist.z * o->faces[i].c.z * o->faces[i].surface.z));

		o->faces[i].df = d;
#endif
	}
}

void PBound(struct Volume *v, struct Fluid *fluid)
{
	for (int j = 0; j < 4; ++j) {
		struct Face *fb = v->faces[j];

		if (!fb->boundary)
			continue;

		vec3 s = vec3Outwards(&v->r,
							  &fb->r,
							  &fb->surface);

		vec3 cv;

		switch (fb->boundary) {
			case open:
			case noSlip: // no-slip wall
			case slip: // slip wall
			case vInlet: // velocity inlet
			case vOutlet: // velocity outlet
				break;
			case pInlet: // pressure inlet
				/*fb->mRate = p_getMrate(v, fb, fluid->rho);

				cv = vec3Copy(&fb->v);
				vec3Mul(&cv, fluid->rho);*/

				/*v->pa += fluid->rho * fb->mRate * fb->df /
						 (fb->mRate - fb->df * vec3Dot(&cv, &cv));*/

				v->pa += fluid->rho * fb->df;

				v->pb -= p_getMrate(v, fb, fluid->rho);
				break;
			case pOutlet: // pressure outlet
				v->pa += fluid->rho * fb->df;

				v->pb -= p_getMrate(v, fb, fluid->rho);
				break;
		}
	}
}

void p_computePBoundCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		PBound(&o->volumes[i], &o->fluid);
	}
}

void VBoundB(struct Volume *v, struct Fluid *fluid)
{
	for (int j = 0; j < 4; ++j) {
		struct Face *fb = v->faces[j];

		if (!fb->boundary)
			continue;

		vec3 s = vec3Outwards(&v->r,
							  &fb->r,
							  &fb->surface);

		real sl = vec3Len(&s);

		vec3 n = vec3Outwards(&v->r,
							  &fb->r,
							  &fb->normal);

		vec3 dcb = vec3Copy(&fb->volDist);

		real dn = vec3Dot(&dcb, &n);

		real a;

		vec3 cv;

		switch (fb->boundary) {
			case noSlip: // no-slip wall
				v->vb.x += (fluid->mu * sl / dn) *
						   (fb->v.x * (1.0 - n.x * n.x) +
							(v->v.y - fb->v.y) * n.y * n.x +
							(v->v.z - fb->v.z) * n.z * n.x
						   );

				v->vb.y += (fluid->mu * sl / dn) *
						   ((v->v.x - fb->v.x) * n.y * n.x +
							fb->v.y * (1.0 - n.y * n.y) +
							(v->v.z - fb->v.z) * n.z * n.x
						   );

				v->vb.z += (fluid->mu * sl / dn) *
						   ((v->v.x - fb->v.x) * n.z * n.x +
							(v->v.y - fb->v.y) * n.y * n.x +
							fb->v.z * (1.0 - n.z * n.z)
						   );
				break;
			case slip: // slip wall
				break;
			case vInlet: // velocity inlet
			case vOutlet: // velocity outlet
				a = p_getVFaceCoeff(fb, v, fluid);

				cv = vec3Copy(&fb->normal); // constant velocity
				vec3Mul(&cv, fb->constantV);

				v->vb.x -= a * cv.x;
				v->vb.y -= a * cv.y;
				v->vb.z -= a * cv.z;
				break;
			case pInlet: // pressure inlet
				a = p_getVFaceCoeff(fb, v, fluid);

				v->vb.x -= a * fb->vn.x;
				v->vb.y -= a * fb->vn.y;
				v->vb.z -= a * fb->vn.z;
				break;
			case pOutlet: // pressure outlet
				fb->mRate = p_getMrate(v, fb, fluid->rho);

				cv = mat3DotV(&fb->vGrad, &fb->volDist);

				v->vb.x -= cv.x * fb->mRate;
				v->vb.y -= cv.y * fb->mRate;
				v->vb.z -= cv.z * fb->mRate;
				break;
		}
	}
}

void VBoundA(struct Volume *v, struct Fluid *fluid)
{
	for (int j = 0; j < 4; ++j) { // twice for middle boundary?
		struct Face *fb = v->faces[j];

		if (!fb->boundary)
			continue;

		vec3 s = vec3Outwards(&v->r,
							  &fb->r,
							  &fb->surface);

		real sl = vec3Len(&s);

		vec3 n = vec3Outwards(&v->r,
							  &fb->r,
							  &fb->normal);

		vec3 dcb = vec3Copy(&fb->volDist);

		real dn = vec3Dot(&dcb, &n);

		real a;

		vec3 cv;

		switch (fb->boundary) {
			case noSlip: // no-slip wall
				v->va.x += (fluid->mu * sl / dn) * (1.0 - n.x * n.x);
				v->va.y += (fluid->mu * sl / dn) * (1.0 - n.y * n.y);
				v->va.z += (fluid->mu * sl / dn) * (1.0 - n.z * n.z);
				break;
			case slip: // slip wall
				break;
			case vInlet: // velocity inlet
			case vOutlet: // velocity outlet
				break;
			case pInlet: // pressure inlet
				break;
			case pOutlet: // pressure outlet
				fb->mRate = p_getMrate(v, fb, fluid->rho);

				v->va.x += fb->mRate;
				v->va.y += fb->mRate;
				v->va.z += fb->mRate;
				break;
		}
	}
}

void p_computeVBoundCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		VBoundA(&o->volumes[i], &o->fluid);
		VBoundB(&o->volumes[i], &o->fluid);
	}
}

void p_computePCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].pa = 0.0;
		for (int j = 0; j < 4; ++j) {
			if (o->volumes[i].faces[j]->pSkip)
				continue;

			o->volumes[i].pa += o->fluid.rho * o->volumes[i].faces[j]->df;
		}

		o->volumes[i].pb = 0.0;
		for (int j = 0; j < 4; ++j) {
			if (o->volumes[i].faces[j]->pSkip)
				continue;

			o->volumes[i].pb -= p_getMrate(&o->volumes[i], o->volumes[i].faces[j], o->fluid.rho);
		}

		PBound(&o->volumes[i], &o->fluid);
	}
}

// gradient integral
vec3 pGradInt(struct Volume *vol)
{
	vec3 r = nvec3();

	for (int i = 0; i < 4; ++i) {
		vec3 s = vec3Outwards(&vol->r,
							  &vol->faces[i]->r,
							  &vol->faces[i]->surface);

		vec3Mul(&s, vol->faces[i]->p);
		vec3Add(&r, &s);
	}

	return r;
}

void getFaceVFlux(struct Face *f, struct Volume *v, struct Fluid *fluid)
{
	f->flux = fluid->mu * vec3Len(&f->surfaceE) / vec3Len(&f->volDist);
	f->flux = fluid->mu * vec3Len(&f->surfaceE) / vec3Len(&f->volDist);
	f->flux = fluid->mu * vec3Len(&f->surfaceE) / vec3Len(&f->volDist);

	real ts = vec3Sign(&v->r,
					   &f->r,
					   &f->surfaceE);

	vec3 vt = vec3Copy(&f->surfaceT);
	vec3Mul(&vt, ts);

	f->vFlux = mat3DotV(&f->vGrad, &vt);
	vec3Mul(&f->vFlux, -fluid->mu);
}

void p_computeVCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].va.x = o->volumes[i].flux;
		o->volumes[i].va.y = o->volumes[i].flux;
		o->volumes[i].va.z = o->volumes[i].flux;

		for (int j = 0; j < 4; ++j) {
			if (o->volumes[i].faces[j]->vSkip)
				continue;

			real mr = max(p_getMrate(&o->volumes[i], o->volumes[i].faces[j], o->fluid.rho), 0.0);

			o->volumes[i].va.x += o->volumes[i].faces[j]->flux + mr;
			o->volumes[i].va.y += o->volumes[i].faces[j]->flux + mr;
			o->volumes[i].va.z += o->volumes[i].faces[j]->flux + mr;
		}

		VBoundA(&o->volumes[i], &o->fluid);

		o->volumes[i].vb = nvec3();
		vec3Sub(&o->volumes[i].vb, &o->volumes[i].vFlux);

		vec3 b0;
#if 0
		b0 = pGradInt(&o->volumes[i]);
#else
		b0 = vec3Copy(&o->volumes[i].pGrad);
		vec3Mul(&b0, o->volumes[i].vol);
#endif

		vec3 b1 = nvec3();
		vec3 b2 = nvec3();

		vec3 br = vec3Copy(&o->volumes[i].vn); // br = (1-vr) / vr * va * v
		br.x *= (1.0 - o->vRelax) / o->vRelax * o->volumes[i].va.x;
		br.y *= (1.0 - o->vRelax) / o->vRelax * o->volumes[i].va.y;
		br.z *= (1.0 - o->vRelax) / o->vRelax * o->volumes[i].va.z;

		o->volumes[i].va.x /= o->vRelax;
		o->volumes[i].va.y /= o->vRelax;
		o->volumes[i].va.z /= o->vRelax;

		for (int j = 0; j < 4; ++j) {
			if (o->volumes[i].faces[j]->vSkip)
				continue;

			if (convsch > 0) {
				vec3 cf = vec3Copy(&o->volumes[i].faces[j]->conFlux); // convection scheme flux
				
				real mr = p_getMrate(&o->volumes[i], o->volumes[i].faces[j], o->fluid.rho);
				
				vec3Mul(&cf, mr);
				
				vec3Add(&b1, &cf);
			}

			getFaceVFlux(o->volumes[i].faces[j], &o->volumes[i], &o->fluid);
			vec3Add(&b1, &o->volumes[i].faces[j]->vFlux);

			vec3 s = vec3Outwards(&o->volumes[i].r,
								  &o->volumes[i].faces[j]->r,
								  &o->volumes[i].faces[j]->surface);

			vec3 v1 = mat3MulV(&o->volumes[i].faces[j]->vGrad, &s);
			vec3Mul(&v1, o->fluid.mu);

			vec3Add(&b2, &v1);
		}

		vec3Sub(&o->volumes[i].vb, &b0);
		vec3Sub(&o->volumes[i].vb, &b1);
		vec3Add(&o->volumes[i].vb, &b2);

		vec3Add(&o->volumes[i].vb, &br);
	}

	for (int i = 0; i < o->volNum; ++i)
		VBoundB(&o->volumes[i], &o->fluid);
}

void p_computeVFaceCoeffs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		for (int j = 0; j < o->volumes[i].neiNum; ++j) {
			int n = o->volumes[i].neighbours[j]->index;

			if (n > o->volNum)
				continue;

			struct Face *connecting = p_connectingFace(&o->volumes[i], &o->volumes[n]);

			p_computeVFaceCoeff(connecting, &o->volumes[n], &o->fluid);
		}
	}
}

void p_computeFaceFs(Obj *o, int in, int tn)
{
	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].conFlux = nvec3();
	}

	if (convsch > 0) {
		for (int i = 0; i < o->volNum && (in > 0 || CONVF) && (tn > 0 || CONVT); ++i) { // convection scheme
			struct Face *uf = getUpwindFace(&o->volumes[i]);
			struct Volume *uv = getUpwindVol(uf);
			
			uf->conFlux = vec3Copy(&o->volumes[i].v);
			
			vec3 rfc = vec3Copy(&uf->r);
			vec3Sub(&rfc, &o->volumes[i].r);
			
			vec3 ucu = mat3DotV(&uv->vGrad, &rfc); // second-order upwind
			
			vec3Sub(&uf->conFlux, &uf->v);
		}
	}

	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].flux = o->fluid.mu * vec3Len(&o->faces[i].surfaceE) / vec3Len(&o->faces[i].volDist);
	}
}

void p_computeVolFs(Obj *o)
{
	for (int i = 0; i < o->volNum; ++i) {
		o->volumes[i].flux = (o->fluid.rho * o->volumes[i].vol) / o->dt;

		o->volumes[i].vFlux = vec3Copy(&o->volumes[i].vtn);
		vec3Mul(&o->volumes[i].vFlux, -(o->fluid.rho * o->volumes[i].vol) / o->dt);

		vec3 s = vec3Copy(&o->fluid.f);
		vec3Add(&s, &o->volumes[i].s);

		vec3Mul(&s, o->volumes[i].vol);

		vec3Sub(&o->volumes[i].vFlux, &s);
	}
}

void decomposedET(struct Face *f)
{
	vec3 e = vec3Copy(&f->volDist);
	vec3Normalize(&e);

	vec3 s = vec3Outwards(&f->thisVol[0]->r,
						  &f->r,
						  &f->surface);

#if MINDC
	real vd = vec3Dot(&e, &s); // minimum correction

	f->surfaceE = vec3Copy(&e);
	vec3Mul(&f->surfaceE, vd);
#elif ORTHODC
	f->surfaceE = vec3Copy(&e); // orthogonal
	vec3Mul(&f->surfaceE, vec3Len(&s));
#elif ORDC
	f->surfaceE = vec3Copy(&e); // over-relaxed

	vec3Mul(&f->surfaceE, vec3Len(&s) * vec3Len(&s) / (vec3Dot(&e, &s)));
#endif

	f->surfaceT = vec3Copy(&s);
	vec3Sub(&f->surfaceT, &f->surfaceE);
}

void p_decomposeSurfs(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1) {
			vec3 d = vec3Copy(&o->faces[i].r);
			vec3Sub(&d, &o->faces[i].thisVol[0]->r);

			o->faces[i].volDist = vec3Copy(&d);

			decomposedET(&o->faces[i]);
		}
	}
	for (int i = 0; i < o->volNum; ++i) {
		for (int jj = 0; jj < o->volumes[i].neiNum; ++jj) {
			int j = o->volumes[i].neighbours[jj]->index;
			struct Face *c = p_connectingFace(&o->volumes[i], &o->volumes[j]);
			if (c == NULL)
				continue;

			vec3 e = vec3Copy(&c->thisVol[1]->r);
			vec3Sub(&e, &c->thisVol[0]->r);
			c->volDist = vec3Copy(&e);

			decomposedET(c);
		}
	}
}
