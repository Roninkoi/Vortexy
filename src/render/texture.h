#ifndef TEXTURE_H
#define TEXTURE_H

#include <phys/field.h>

struct Texture {
};

void loadTex(struct Texture* t, char* path);
void fieldToTex(struct Field* f, struct Texture* t);

#endif
