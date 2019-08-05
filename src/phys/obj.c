#include "obj.h"

void p_loadObj(Obj *o, char *path)
{
	p_loadMesh(&o->mesh, path, 1, 0);
}
