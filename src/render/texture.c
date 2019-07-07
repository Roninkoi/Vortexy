#include "render/texture.h"
#include "util/texLoader.h"

void r_loadTex(struct Texture *t, char *path)
{
	struct Field f;

	ppmLoader(&f, path);

	r_fieldToTex(&f, t);
}

void r_fieldToTex(struct Field *f, struct Texture *t)
{
}
