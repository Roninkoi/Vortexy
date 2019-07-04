#include "phys/mesh.h"
#include "util/objParser.h"

void p_loadMesh(struct Mesh *m, char *path)
{
  objParser(m, path);
}
