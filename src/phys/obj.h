#ifndef OBJ_H
#define OBJ_H

#include "mesh.h"
#include "phys.h"
#include "volume.h"
#include "fluid.h"

typedef struct {
	Mesh mesh; // physics mesh
	Mesh renderMesh;

	struct Face *faces;
	int faceNum;
	
	struct Volume *volumes;
	int volNum;

	struct Fluid fluid;
	struct Phys phys;
} Obj;

void p_loadObj(Obj *o, char *meshPath, char *fluidPath);

void p_destroyObj(Obj *o);

#endif
