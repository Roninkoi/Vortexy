#ifndef MESH_H
#define MESH_H

#include "mat.h"

// triangular mesh
typedef struct {
	float *vertData;
	float *vertData0; // untransformed
	float *texData;
	float *normData;
	float *colData;

	int *indData;

	int vertNum;
	int texNum;
	int normNum;
	int colNum;

	int indNum;
} Mesh;

void p_meshInit(Mesh *m);

void p_loadMesh(Mesh *m, char *path);

void p_meshTransform(Mesh *m, mat4 mat);

void p_meshDestroy(Mesh *m);

#endif
