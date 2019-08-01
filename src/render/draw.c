#include "render/render.h"

void r_draw(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	for (int i = 0; i < 100; ++i) {
		r_drawLine(r, p_vec4(0.0f, 0.0f, 0.0f, 0.0f), p_vec4(cosf((float) i / 100.0f), sinf((float) i / 100.0f), 0.0f, 0.0f), p_vec4(0.0f, 4.0f, 0.0f, 1.0f));
	}
	r_render(r);

	for (int i = 0; i < s->objNum; ++i) {
		r_drawMesh(r, &s->objs[i].mesh);
	}

	r_render(r);
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

	for (int i = 0; i < m->indNum; i += 3) { // expand tris into line segs
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

void r_drawLine(struct Renderer *r, vec4 v0, vec4 v1, vec4 col)
{
	r->drawMode = GL_LINES;

	float vd[8] = {
			v0.x, v0.y, v0.z, v0.w,
			v1.x, v1.y, v1.z, v1.w
	};

	float td[8] = {
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
	};

	float nd[8] = {
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
	};

	float cd[8] = {
			col.x, col.y, col.z, col.w,
			col.x, col.y, col.z, col.w
	};

	int id[2] = {0, 1};

	r_add(r, vd, td, nd, cd, id, 8, 2);
}
