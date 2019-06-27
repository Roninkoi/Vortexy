#include <render/texture.h>
#include <util/texLoader.h>

void loadTex(struct Texture* t, char* path)
{
  struct Field f;

  ppmLoader(&f, path);

  fieldToTex(&f, t);
}

void fieldToTex(struct Field* f, struct Texture* t)
{
}
