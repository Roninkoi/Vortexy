#ifndef FIELD_H
#define FIELD_H

#include "phys/vec.h"

// vector field defined on a mesh
// (texture on GPU)
typedef struct {
	unsigned int res; // field resolution
	vec4 **f; // res x res
} Field;

void p_fieldInit(Field *f, unsigned int r);

void p_fieldSet(Field *f, float x, float y, float z, float w);

void p_fieldPrint(Field *f);

void p_fieldLoader(Field *f, int width, int height, unsigned char *data);

#endif
