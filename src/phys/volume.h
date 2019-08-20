#ifndef VOLUME_H
#define VOLUME_H

#include "mesh.h"

struct Volume;

struct Face { // triangle
	float verts[12];
	int inds[3];

	//int vertNum;
	//int indNum;

	vec4 flux; // flux though face

	vec4 normal;
	vec4 centroid;

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

	int index;

	float vol;

	vec4 centroid;
};

// allocates faces which are returned
struct Face *p_loadFaces(Mesh *m, int *faceNum);

struct Volume *p_loadVolumes(struct Face *f, int faceNum, int *volNum);

#endif
