#ifndef OBJ_H
#define OBJ_H

#include "mesh.h"
#include "volume.h"
#include "fluid.h"

typedef struct {
	Mesh mesh;
	char *meshPath;

	struct Face *faces;
	int faceNum;
	
	struct Volume *volumes;
	int volNum;

	struct Fluid fluid;

	real pRelax;
	real vRelax;

	real dt;
	real t;

	mat a;
	mat b;
	mat vx;
	mat vy;
	mat vz;
	mat vp;
} Obj;

void p_loadObj(Obj *o, char *fluidPath, int mode);

void p_reloadObj(Obj *o, char *fluidPath);

void p_loadObjMesh(Obj *o);

void p_loadObjVolumes(Obj *o);

void p_destroyObj(Obj *o);

#endif
