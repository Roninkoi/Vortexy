#include "gradient.h"

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

void p_pGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceP(f);
	}

	p_pGrad(o);

	for (int k = 0; k < gradIt; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue; // lower order estimate at boundary?
			}

			real wf = getGWF(f);

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

void p_vGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceV(f, 0);
	}

	p_vGrad(o);

	for (int k = 0; k < gradIt; ++k) {
		for (int i = 0; i < o->faceNum; ++i) {
			struct Face *f = &o->faces[i];

			if (f->vNum == 0)
				continue;

			if (f->vNum < 2) {
				continue;
			}

			real wf = getGWF(f);

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

void p_pcGrads(Obj *o)
{
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_facePC(f);
	}

	p_pcGrad(o);

	for (int k = 0; k < gradIt; ++k) {
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

void p_pGradsh(Obj *o)
{
	halfGWF = 1;
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceP(f);
	}
	halfGWF = 0;

	p_pGrad(o);

	for (int k = 0; k < gradIt; ++k) {
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

void p_vGradsh(Obj *o)
{
	halfGWF = 1;
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_faceV(f, 0);
	}
	halfGWF = 0;

	p_vGrad(o);

	for (int k = 0; k < gradIt; ++k) {
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

void p_pcGradsh(Obj *o)
{
	halfGWF = 1;
	for (int i = 0; i < o->faceNum; ++i) {
		struct Face *f = &o->faces[i];

		p_facePC(f);
	}
	halfGWF = 0;

	p_pcGrad(o);

	for (int k = 0; k < gradIt; ++k) {
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
