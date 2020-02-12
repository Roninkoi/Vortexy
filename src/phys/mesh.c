#include "mesh.h"
#include "util/objParser.h"
#include "util/meshParser.h"

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

void p_meshSetCol(Mesh *m, float r, float g, float b, float a)
{
	for (int i = 0; i < m->colNum; i += 4) {
		m->colData[i + 0] = r;
		m->colData[i + 1] = g;
		m->colData[i + 2] = b;
		m->colData[i + 3] = a;
	}
}

void p_meshTransform(Mesh *m, mat4 *mat)
{
	for (int i = 0; i < m->vertNum; i += 4) {
		vec4 v = Vec4(m->vertData0[i + 0],
						m->vertData0[i + 1],
						m->vertData0[i + 2],
						m->vertData0[i + 3]);

		vec4 vt = mat4MulV(mat, &v);

		m->vertData[i + 0] = vt.x;
		m->vertData[i + 1] = vt.y;
		m->vertData[i + 2] = vt.z;
		m->vertData[i + 3] = vt.w;
	}
}

void p_loadMesh(Mesh *m, char *path, int optimize, int loadTex)
{
	if (strstr(path, ".obj") != NULL)
		*m = objParser(path, optimize, loadTex);

	if (strstr(path, ".mesh") != NULL)
		*m = meshParser(path);

	m->vertData0 = floatCopy(m->vertData, m->vertNum);
}

void p_destroyMesh(Mesh *m)
{
	free(m->vertData0);
	free(m->vertData);
	
	free(m->texData);
	free(m->normData);
	free(m->colData);

	free(m->indData);
}
