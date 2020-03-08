#ifndef VOLUME_H
#define VOLUME_H

#include "mesh.h"

struct Volume;

struct Face { // triangle
	float verts[12];
	int inds[3];

	//int vertNum;
	//int indNum;

	vec3 v; // velocity
	float p; // pressure
	vec3 vi; // velocity
	float pi; // pressure

	float initialV; // initial conditions
	float initialP;
	float constantV;
	float constantP;
	
	int boundary; // boundary condition

	vec3 normal; // unit normal
	vec3 surface; // surface vector with area
	vec3 surfaceE; // big
	vec3 surfaceT; // small

	vec3 centroid;

	vec3 volDist; // distance between volumes

	float mRate; // mass flow rate
	vec3 mFlux; // mass flux
	vec3 vFlux; // velocity flux
	vec3 flux; // face flux

	vec3 conFlux;

	float df;

	float pc;

	vec3 d;

	vec3 ed;

	mat vGrad; // velocity gradient
	vec3 pGrad; // pressure gradient
	mat vGradI; // interpolated
	vec3 pGradI; // interpolated

	float area;

	int index; // index in face array

	struct Face **connecting; // faces sharing edge
	int conNum;

	struct Volume *thisVol[2]; // volumes this face is a part of
	int vNum;
};

struct Volume { // tetrahedron
	struct Face *faces[4];
	struct Volume *neighbours[4]; // neighbouring volume elements

	int neiNum;

	//int faceNum;

	vec3 v; // volume
	float p; // pressure

	vec3 s; // source

	mat vGrad; // volume gradient
	vec3 pGrad; // pressure gradient
	vec3 pcGrad; // pressure correction gradient

	float mRate; // mass flow rate
	vec3 mFlux; // mass flux
	vec3 vFlux; // velocity flux
	float flux; // volume flux

	vec3 va; // velocity coefficients
	vec3 vb;

	float pa; // pressure coefficients
	float pb;

	vec3 d;

	float pc;

	int index;

	float vol;

	vec3 centroid;
};

// allocates faces which are returned
struct Face *p_loadFaces(Mesh *m, int *faceNum, int mode);

struct Volume *p_loadVolumes(struct Face *f, int faceNum, int *volNum);

void p_destroyFaces(struct Face *f, int faceNum);

void p_destroyVolumes(struct Volume *v, int volNum);

int p_volCmp(struct Volume *v0, struct Volume *v1);

int p_faceCmp(struct Face *f0, struct Face *f1);

struct Face *p_connectingFace(struct Volume *v0, struct Volume *v1);

#endif
