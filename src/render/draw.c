#include "render/render.h"

void r_draw(struct Renderer *r)
{

}

void r_render(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	/*r->vertices[0] = 0.0f;
	r->vertices[1] = 0.0f;
	r->vertices[2] = 0.0f;
	r->vertices[3] = 0.0f;

	r->vertices[4] = 1.0f;
	r->vertices[5] = 0.0f;
	r->vertices[6] = 0.0f;
	r->vertices[7] = 0.0f;

	r->vertices[8] = 1.0f;
	r->vertices[9] = 1.0f;
	r->vertices[10] = 0.0f;
	r->vertices[11] = 0.0f;

	r->texes[0] = 0.0f;
	r->texes[1] = 0.0f;
	r->texes[2] = 1.0f;
	r->texes[3] = 1.0f;

	r->texes[4] = 1.0f;
	r->texes[5] = 0.0f;
	r->texes[6] = 1.0f;
	r->texes[7] = 1.0f;

	r->texes[8] = 1.0f;
	r->texes[9] = 1.0f;
	r->texes[10] = 1.0f;
	r->texes[11] = 1.0f;

	r->vertexNum = 3 * 4;

	r->indices[0] = 0;
	r->indices[1] = 1;
	r->indices[2] = 2;

	r->indexNum = 3;*/

	for (int i = 0; i < s->objNum; ++i) {
		r_drawMesh(r, &s->objs[i].mesh);
	}

	r_copy(r);

	r_flush(r);
}

void r_drawMesh(struct Renderer *r, Mesh *m)
{
	r->drawMode = GL_TRIANGLES;

	r_add(r, m->vertData, m->texData, m->normData, m->colData, m->indData,
		  m->vertNum, m->indNum);
}

void r_drawWireMesh(struct Renderer *r, Mesh *m)
{
	r->drawMode = GL_LINES;

	int newInds[2 * m->indNum];

	for (int i = 0; i < m->indNum; i += 3) {
		newInds[2 * i + 0] = m->indData[i + 0];
		newInds[2 * i + 1] = m->indData[i + 1];

		newInds[2 * i + 2] = m->indData[i + 1];
		newInds[2 * i + 3] = m->indData[i + 2];

		newInds[2 * i + 4] = m->indData[i + 2];
		newInds[2 * i + 5] = m->indData[i + 0];
	}

	r_add(r, m->vertData, m->texData, m->normData, m->colData, &newInds[0],
		  m->vertNum, m->indNum * 2);
}
