#include "phys/mesh.h"
#include "util/objParser.h"

void p_meshInit(Mesh *m)
{
	m->vertData = NULL;
	m->texData = NULL;
	m->normData = NULL;
	m->colData = NULL;

	m->indData = NULL;

	m->vertNum = 0;
	m->texNum = 0;
	m->normNum = 0;
	m->colNum = 0;

	m->indNum = 0;
}

void p_loadMesh(Mesh *m, char *path)
{
	*m = objParser(path);
}

void p_meshDestroy(Mesh *m)
{
	free(m->vertData);
	free(m->texData);
	free(m->normData);
	free(m->colData);

	free(m->indData);
}