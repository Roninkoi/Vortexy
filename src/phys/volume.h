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
	real p; // pressure
	vec3 vi; // velocity
	vec3 vin; // velocity
	real pi; // pressure

	real initialV; // initial conditions
	real initialP;
	real constantV;
	real constantP;
	
	int boundary; // boundary condition

	int isWall;

	vec3 normal; // unit normal
	vec3 surface; // surface vector with area
	vec3 surfaceE; // big
	vec3 surfaceT; // small

	vec3 centroid;

	vec3 volDist; // distance between volumes

	real mRate; // mass flow rate
	vec3 mFlux; // mass flux
	vec3 vFlux; // velocity flux
	vec3 flux; // face flux

	vec3 conFlux;

	real df;

	real pc;
	real pci;

	vec3 d;

	vec3 ed;

	mat vGrad; // velocity gradient
	vec3 pGrad; // pressure gradient
	mat vGradI; // interpolated
	vec3 pGradI; // interpolated

	vec3 va; // velocity coefficients

	real pa; // pressure coefficients

	real area;

	int index; // index in face array

	struct Face **connecting; // faces sharing edge
	int conNum;

	struct Volume *thisVol[2]; // volumes this face is a part of
	int vNum;
};

void faceInit(struct Face *f)
{
	f->centroid = nvec3();
	f->flux = nvec3();
	f->d = nvec3();
	f->mRate = 0.0;
	f->boundary = 0;
	f->isWall = 0;
	f->initialP = 0.0;
	f->initialV = 0.0;
	f->constantP = 0.0;
	f->constantV = 0.0;
	f->vFlux = nvec3();
	f->mFlux = nvec3();
	f->conFlux = nvec3();
	f->pGrad = nvec3();
	f->vGrad = Mat(0.0, 3, 3);
	f->pGradI = nvec3();
	f->vGradI = Mat(0.0, 3, 3);
	f->v = nvec3();
	f->vn = nvec3();
	f->p = 0.0;
	f->va = nvec3();
	f->pa = 0.0;
	f->pc = 0.0;
	f->pci = 0.0;
	f->df = 0.0;
	f->vi = nvec3();
	f->vin = nvec3();
	f->pi = 0.0;
	f->vNum = 0;
	f->thisVol[0] = NULL;
	f->thisVol[1] = NULL;
}

struct Volume { // tetrahedron
	struct Face *faces[4];
	struct Volume *neighbours[4]; // neighbouring volume elements

	int neiNum;

	//int faceNum;

	vec3 v; // volume
	real p; // pressure

	vec3 s; // source

	mat vGrad; // volume gradient
	vec3 pGrad; // pressure gradient
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

	vec3 d;

	real pc;

	int index;

	real vol;

	vec3 centroid;
};

void volInit(struct Volume *v)
{
	v->flux = 0.0;
	v->mRate = 0.0;
	v->mFlux = nvec3();
	v->vFlux = nvec3();
	v->pGrad = nvec3();
	v->pcGrad = nvec3();
	v->shearStress = nvec3();
	v->d = nvec3();
	v->s = nvec3();
	v->vGrad = Mat(0.0, 3, 3);
	v->hasBoundary = 0;
}

// allocates faces which are returned
struct Face *p_loadFaces(Mesh *m, int *faceNum, int mode);

struct Volume *p_loadVolumes(struct Face *f, int faceNum, int *volNum);

void p_destroyFaces(struct Face *f, int faceNum);

void p_destroyVolumes(struct Volume *v, int volNum);

int p_volCmp(struct Volume *v0, struct Volume *v1);

int p_faceCmp(struct Face *f0, struct Face *f1);

struct Face *p_connectingFace(struct Volume *v0, struct Volume *v1);

#endif
