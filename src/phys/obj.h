#ifndef OBJ_H
#define OBJ_H

#include "mesh.h"
#include "field.h"
#include "phys.h"
#include "volume.h"

typedef struct {
	Mesh mesh; // physics mesh
	Mesh renderMesh;

	struct Face faces;
	struct Volume volumes;
	
	Field field;
	struct Phys phys;
} Obj;

void p_loadObj(Obj *o, char *path);

#endif
