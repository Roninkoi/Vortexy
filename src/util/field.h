#ifndef FIELD_H
#define FIELD_H

#include "vec.h"

// vector field
typedef struct {
	unsigned int res; // field resolution
	vec4 **f; // res x res
} Field;

void fieldInit(Field *f, unsigned int r);

void fieldSet(Field *f, float x, float y, float z, float w);

void fieldPrint(Field *f);

void fieldLoader(Field *f, int width, int height, unsigned char *data);

unsigned char *fieldBytes(Field *f);

#endif
