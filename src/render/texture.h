#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include "phys/field.h"

typedef struct {
	char *path;

	int w;
	int h;

	GLuint tex;
} Texture;

void r_loadTex(Texture *t, char *path);

void r_fieldToTex(Field *f, Texture *t);

void r_destroyTex(Texture *t);

#endif
