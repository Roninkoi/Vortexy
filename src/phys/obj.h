#ifndef OBJ_H
#define OBJ_H

#include "mesh.h"
#include "volume.h"
#include "fluid.h"

typedef struct {
	Mesh mesh; // physics mesh
	Mesh renderMesh;

	struct Face *faces;
	int faceNum;
	
	struct Volume *volumes;
	int volNum;

	vec3 f;

	struct Fluid fluid;

	float dt;
	float t;

	mat a;
	mat b;
	mat v;
} Obj;

void p_loadObj(Obj *o, char *meshPath, char *fluidPath);

void p_destroyObj(Obj *o);

#endif
