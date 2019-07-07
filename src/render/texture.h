#ifndef TEXTURE_H
#define TEXTURE_H

#include "phys/field.h"

struct Texture {
};

void r_loadTex(struct Texture *t, char *path);
void r_fieldToTex(struct Field *f, struct Texture *t);

#endif
