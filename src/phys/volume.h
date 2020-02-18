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

	float initial;
	float constant;
	int boundary;

	vec3 normal; // unit normal
	vec3 surface; // surface vector with area
	vec3 surfaceE; // big
	vec3 surfaceT; // small

	vec3 centroid;

	vec3 volDist; // distance between volumes

	vec3 mFlux; // mass flux
	vec3 vFlux; // velocity flux
	float flux; // face flux

	mat vGrad; // velocity gradient
	vec3 pGrad; // pressure gradient

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

	mat vGrad; // volume gradient
	vec3 pGrad; // pressure gradient

	vec3 mFlux; // mass flux
	vec3 vFlux; // velocity flux
	float flux; // volume flux

	float a;
	vec3 b;

	int index;

	float vol;

	vec3 centroid;
};

// allocates faces which are returned
struct Face *p_loadFaces(Mesh *m, int *faceNum);

struct Volume *p_loadVolumes(struct Face *f, int faceNum, int *volNum);

int p_volCmp(struct Volume *v0, struct Volume *v1);

int p_faceCmp(struct Face *f0, struct Face *f1);

struct Face *p_connectingFace(struct Volume *v0, struct Volume *v1);

#endif
