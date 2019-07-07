#ifndef MESH_H
#define MESH_H

// triangular mesh
struct Mesh {
	float vertexData;
	float texData;
	float normalData;

	int indexData;
};

void p_loadMesh(struct Mesh *m, char *path);

#endif
