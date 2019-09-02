#ifndef VOLUME_H
#define VOLUME_H

#include "mesh.h"
#include "phys.h"

struct Volume;

struct Face { // triangle
	float verts[12];
	int inds[3];

	//int vertNum;
	//int indNum;

	vec3 mFlux; // flux though face

	float initial;
	float constant;
	int boundary;

	vec3 normal;
	vec3 centroid;

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
	struct Phys phys;
	
	vec3 mFlux;
	vec3 vGrad; // gradients
	vec3 pGrad;

	int index;

	float vol;

	vec3 centroid;
};

// allocates faces which are returned
struct Face *p_loadFaces(Mesh *m, int *faceNum);

struct Volume *p_loadVolumes(struct Face *f, int faceNum, int *volNum);

#endif
