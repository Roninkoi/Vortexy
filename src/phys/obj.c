#include "obj.h"
#include "util/cfgParser.h"

void p_loadObj(Obj *o, char *fluidPath, int mode)
{
	o->faces = NULL;
	o->volumes = NULL;

	// default values
	o->t = 0.0f;
	o->dt = 1.0f;
	o->vRelax = 0.7;
	o->pRelax = 0.3;

	printf("Loading %s\n", fluidPath);

	// load fluid mesh
	fluidMeshParser(o, fluidPath);

	p_loadObjMesh(o);
	o->faces = p_loadFaces(&o->mesh, &o->faceNum, mode);

	if (mode == 1) { // if only rendering, don't compute fvm
		fluidParser(o, fluidPath);

		return;
	}

	p_loadObjVolumes(o);

	p_reloadObj(o, fluidPath);
}

void p_reloadObj(Obj *o, char *fluidPath)
{
	for (int i = 0; i < o->faceNum; ++i) {
		o->faces[i].initialV = 0.0;
		o->faces[i].constantV = 0.0;

		o->faces[i].initialP = 0.0;
		o->faces[i].constantP = 0.0;

		o->faces[i].boundary = -1;

		for (int j = 0; j < o->faces[i].vNum; ++j)
			o->faces[i].thisVol[j]->hasBoundary = 0;
	}

	fluidParser(o, fluidPath);

	for (int i = 0; i < o->faceNum; ++i) {
		if (o->faces[i].vNum == 1 && o->faces[i].boundary < 0) {
			o->faces[i].boundary = o->fluid.ebc;
		}

		if (o->faces[i].boundary < 0) {
			o->faces[i].boundary = 0;
		}

		if (o->faces[i].boundary == 1 ||
			o->faces[i].boundary == 2)
			o->faces[i].isWall = 1;

		if (o->faces[i].boundary == 1 ||
			o->faces[i].boundary == 2 ||
			o->faces[i].boundary == 6)
			o->faces[i].vSkip = 1;

		if (o->faces[i].boundary > 0)
			o->faces[i].pSkip = 1;

		o->faces[i].constantP += o->fluid.bp;
		o->faces[i].initialP += o->fluid.bp;

		if (o->faces[i].boundary)
			for (int j = 0; j < o->faces[i].vNum; ++j)
				o->faces[i].thisVol[j]->hasBoundary += 1;
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
