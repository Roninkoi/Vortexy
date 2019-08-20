#include "obj.h"

void p_loadObj(Obj *o, char *path)
{
	o->faces = NULL;
	o->volumes = NULL;
	
	p_loadMesh(&o->mesh, path, 1, 1);

	o->faces = p_loadFaces(&o->mesh, &o->faceNum);

	o->volumes = p_loadVolumes(o->faces, o->faceNum, &o->volNum);
	
	return;
	for (int i = 0; i < o->faceNum; ++i) {
		printf("i %i %i %i\n", o->faces[i].inds[0], o->faces[i].inds[1], o->faces[i].inds[2]);

		for (int j = 0; j < 12; ++j) {
			printf("v %f\n", o->faces[i].verts[j]);
		}

		printf("a %f\n", o->faces[i].area);
	}
}
