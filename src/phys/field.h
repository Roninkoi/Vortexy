#ifndef FIELD_H
#define FIELD_H

#include "phys/vec.h"

// vector field defined on a mesh
// (texture on GPU)
struct Field {
	unsigned int res; // field resolution
	vec4 **f; // res x res
};

void p_fieldInit(struct Field *f, unsigned int r);

void p_fieldSet(struct Field *f, float x, float y, float z, float w);

#endif
