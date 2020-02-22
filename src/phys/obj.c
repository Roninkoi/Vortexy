#include "obj.h"
#include "util/cfgParser.h"

void p_loadObj(Obj *o, char *fluidPath)
{
	o->faces = NULL;
	o->volumes = NULL;

	printf("Loading %s %s\n", fluidPath);

	fluidParser(o, fluidPath);
}

void p_loadObjMesh(Obj *o)
{
	p_loadMesh(&o->mesh, o->meshPath, 1, 1);

	o->faces = p_loadFaces(&o->mesh, &o->faceNum);
	o->volumes = p_loadVolumes(o->faces, o->faceNum, &o->volNum);
}

void p_destroyObj(Obj *o)
{
	p_destroyMesh(&o->mesh);
	p_destroyMesh(&o->renderMesh);

	//p_destroyFaces();
	//p_destroyVolumes();
}
