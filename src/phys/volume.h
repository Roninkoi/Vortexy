#ifndef VOLUME_H
#define VOLUME_H

#include "vec.h"

struct Volume;

struct Face {
	float *verts;
	int *inds;

	int vertNum;
	int indNum;

	vec4 flux; // flux though face

	vec4 normal;
	vec4 centroid;

	struct Face *other; // opposing face
	struct Volume *thisVol; // volume this face is a part of
};

struct Volume {
	struct Face *faces;
	int faceNum;

	float vol;

	vec4 centroid;
};

#endif
