#ifndef VOLUME_H
#define VOLUME_H

#include "mesh.h"

struct Volume;

struct Face { // triangle
	real verts[12];
	int inds[3];

	//int vertNum;
	//int indNum;

	vec3 v; // velocity
	vec3 vn; // velocity
	vec3 vtn; // velocity
	real p; // pressure
	vec3 vI; // velocity
	vec3 vIn; // velocity
	vec3 vItn; // velocity
	real pI; // pressure

	real initialV; // initial conditions
	real initialP;
	real constantV; // boundary values
	real constantP;
	
	int boundary; // boundary condition

	int isWall;

	int vSkip;
	int pSkip;

	vec3 normal; // unit normal
	vec3 surface; // surface vector with area
	vec3 surfaceE; // big
	vec3 surfaceT; // small

	vec3 r; // centroid

	vec3 volDist; // distance between volumes

	real mRate; // mass flow rate
	vec3 mFlux; // mass flux
	vec3 vFlux; // velocity flux
	real flux; // face flux

	vec3 conFlux;

	real df; // s (proj) e

	real pc; // pressure correction
	real pcI; // pressure correction interpolation

	vec3 c; // v / a
	vec3 ctn; // previous time-step c

	mat3 vGrad; // velocity gradient
	mat3 vGradI; // interpolated
	vec3 pGrad; // pressure gradient
	vec3 pGradI; // interpolated
	vec3 pGradn; // pressure gradient

	vec3 va; // velocity coefficients
	real pa; // pressure coefficients

	real area; // area of face

	int index; // index in face array

	struct Face **connecting; // faces sharing edge
	int conNum;

	struct Volume *thisVol[2]; // volumes this face is a part of
	int vNum; // number of neighbouring volumes
};

void faceInit(struct Face *f)
{
	f->v = nvec3();
	f->vn = nvec3();
	f->vtn = nvec3();
	f->p = 0.0;
	f->vI = nvec3();
	f->vIn = nvec3();
	f->vItn = nvec3();
	f->pI = 0.0;

	f->initialV = 0.0;
	f->initialP = 0.0;
	f->constantV = 0.0;
	f->constantP = 0.0;

	f->boundary = 0;

	f->isWall = 0;

	f->vSkip = 0;
	f->pSkip = 0;

	f->normal = nvec3();
	f->surface = nvec3();
	f->surfaceE = nvec3();
	f->surfaceT = nvec3();

	f->r = nvec3();

	f->volDist = nvec3();

	f->mRate = 0.0;
	f->mFlux = nvec3();
	f->vFlux = nvec3();
	f->flux = 0.0;

	f->conFlux = nvec3();

	f->df = 0.0;

	f->pc = 0.0;
	f->pcI = 0.0;

	f->c = nvec3();
	f->ctn = nvec3();

	f->vGrad = nmat3();
	f->vGradI = nmat3();
	f->pGrad = nvec3();
	f->pGradI = nvec3();
	f->pGradn = nvec3();

	f->va = nvec3();
	f->pa = 0.0;

	f->area = 0.0;

	f->index = -1;

	f->connecting = NULL;
	f->conNum = 0;

	f->thisVol[0] = NULL;
	f->thisVol[1] = NULL;

	f->vNum = 0;
}

struct Volume { // tetrahedron
	struct Face *faces[4];
	struct Volume *neighbours[4]; // neighbouring volume elements

	int neiNum;

	//int faceNum;

	vec3 v; // velocity
	vec3 vn; // velocity
	vec3 vtn; // velocity
	real p; // pressure

	vec3 s; // source

	mat3 vGrad; // velocity gradient
	vec3 pGrad; // pressure gradient
	vec3 pGradn; // pressure gradient
	vec3 pcGrad; // pressure correction gradient

	real mRate; // mass flow rate
	vec3 mFlux; // mass flux
	vec3 vFlux; // velocity flux
	real flux; // volume flux

	vec3 shearStress;
	int hasBoundary;

	vec3 va; // velocity coefficients
	vec3 vb;

	real pa; // pressure coefficients
	real pb;

	vec3 c; // v / a

	real pc; // pressure correction

	int index;

	real vol; // volume

	vec3 r; // centroid
};

void volInit(struct Volume *v)
{
	v->faces[0] = NULL;
	v->faces[1] = NULL;
	v->faces[2] = NULL;
	v->faces[3] = NULL;

	v->neighbours[0] = NULL;
	v->neighbours[1] = NULL;
	v->neighbours[2] = NULL;
	v->neighbours[3] = NULL;

	v->neiNum = 0;

	v->v = nvec3();
	v->vn = nvec3();
	v->vtn = nvec3();
	v->p = 0.0;

	v->s = nvec3();

	v->vGrad = nmat3();
	v->pGrad = nvec3();
	v->pGradn = nvec3();
	v->pcGrad = nvec3();

	v->mRate = 0.0;
	v->mFlux = nvec3();
	v->vFlux = nvec3();
	v->flux = 0.0;

	v->shearStress = nvec3();
	v->hasBoundary = 0;

	v->va = nvec3();
	v->vb = nvec3();

	v->pa = 0.0;
	v->pb = 0.0;

	v->c = nvec3();

	v->pc = 0.0;

	v->index = -1;

	v->vol = 0.0;

	v->r = nvec3();
}

// allocates faces which are returned
struct Face *p_loadFaces(Mesh *m, int *faceNum, int mode);

struct Volume *p_loadVolumes(struct Face *f, int faceNum, int *volNum);

void p_destroyFaces(struct Face *f, int faceNum);

void p_destroyVolumes(struct Volume *v, int volNum);

int p_volCmp(struct Volume *v0, struct Volume *v1);

int p_faceCmp(struct Face *f0, struct Face *f1);

struct Face *p_connectingFace(struct Volume *v0, struct Volume *v1);


void destroyFace(struct Face *f)
{
	// nothing dynamically allocated
}

void destroyVolume(struct Volume *v)
{
	// nothing dynamically allocated
}

void p_destroyFaces(struct Face *f, int faceNum)
{
	for (int i = 0; i < faceNum; ++i) {
		destroyFace(&f[i]);
	}

	free(f);
}

void p_destroyVolumes(struct Volume *v, int volNum)
{
	for (int i = 0; i < volNum; ++i) {
		destroyVolume(&v[i]);
	}

	free(v);
}

#endif
