#include "render/render.h"

void r_render(struct Renderer *r)
{
	r_update(r);

	r->vertices[0] = 0.0f;
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

	r->indexNum = 3;

	r_copy(r);

	r_flush(r);
}

void r_drawMesh(Mesh *m)
{
}
