#include "obj.h"
#include "util/cfgParser.h"

void p_loadObj(Obj *o, char *fluidPath, int mode)
{
	o->faces = NULL;
	o->volumes = NULL;

	o->t = 0.0f;

	printf("Loading %s\n", fluidPath);

	fluidMeshParser(o, fluidPath);

	p_loadObjMesh(o);
	o->faces = p_loadFaces(&o->mesh, &o->faceNum, mode);

	if (mode)
		return;
		
	p_loadObjVolumes(o);

	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].initialV = 0.0f;
		o->faces[i].constantV = 0.0f;

		o->faces[i].initialP = 0.0f;
		o->faces[i].constantP = 0.0f;
	}

	fluidParser(o, fluidPath);

	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1) {
			o->faces[i].boundary = o->fluid.ebc;
		}

		if (o->faces[i].boundary)
			for (int j = 0; j < o->faces[i].vNum; ++j)
				o->faces[i].thisVol[j]->hasBoundary += 1;
	}
}

void p_reloadObj(Obj *o, char *fluidPath)
{
	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].initialV = 0.0f;
		o->faces[i].constantV = 0.0f;

		o->faces[i].initialP = 0.0f;
		o->faces[i].constantP = 0.0f;
	}

	fluidParser(o, fluidPath);

	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1) {
			o->faces[i].boundary = o->fluid.ebc;
		}
	}
}

void p_loadObjVolumes(Obj *o)
{
	o->volumes = p_loadVolumes(o->faces, o->faceNum, &o->volNum);
}

void p_loadObjMesh(Obj *o)
{
	p_loadMesh(&o->mesh, o->meshPath, 1, 1);
}

void p_destroyObj(Obj *o)
{
	p_destroyMesh(&o->mesh);

	p_destroyFaces(o->faces, o->faceNum);
	p_destroyVolumes(o->volumes, o->volNum);
}
