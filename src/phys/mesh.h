#ifndef MESH_H
#define MESH_H

// triangular mesh
struct Mesh {
	float vertData;
	float texData;
	float normData;
	float colData;

	int indData;
};

void p_loadMesh(struct Mesh *m, char *path);

#endif
