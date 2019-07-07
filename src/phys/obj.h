#ifndef OBJ_H
#define OBJ_H

#include "phys/mesh.h"
#include "phys/field.h"

struct Obj {
	struct Mesh mesh;
	struct Field field;
};

void p_loadObj(char *path);

#endif
