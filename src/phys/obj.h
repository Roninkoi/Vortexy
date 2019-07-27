#ifndef OBJ_H
#define OBJ_H

#include "phys/mesh.h"
#include "phys/field.h"

typedef struct {
	Mesh mesh;
	Field field;
} Obj;

void p_loadObj(char *path);

#endif
