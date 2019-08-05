#ifndef VOLUME_H
#define VOLUME_H

#include "vec.h"

struct Volume;

struct Face { // triangle
	float *verts;
	int *inds;

	int vertNum;
	int indNum;

	vec4 flux; // flux though face

	vec4 normal;
	vec4 centroid;

	float area;

	//struct Face *other; // opposing face
	struct Volume *thisVol[2]; // volumes this face is a part of
};

struct Volume { // tetrahedron
	struct Face *faces[4];
	struct Volume *neighbours[4]; // neighbouring volume elements
	
	int faceNum;

	float vol;

	vec4 centroid;
};

// loadFaces
// computeArea
// computeFaceCentroid

// loadVolumes
// computeVol
// computeVolumeCentroid
// computeTopology

#endif
