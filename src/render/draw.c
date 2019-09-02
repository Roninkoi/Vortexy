#include "render.h"

void r0(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	r->tex = &r->tex0;
	for (int i = 0; i < s->objNum; ++i) {
		r_drawMesh(r, &s->objs[i].mesh);
	}
	r_render(r);
}

void r1(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		r_drawMesh(r, &s->objs[i].mesh);
	}
	r_render(r);

	glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < s->objNum; ++i) {
		p_meshSetCol(&s->objs[i].mesh, 0.0f, 0.0f, 0.0f, 1.0f);
		r_drawWireMesh(r, &s->objs[i].mesh);
	}

	r_render(r);
}

void r2(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	r->tex = &r->flat;

	int n = 800;
	for (int i = 0; i < n; ++i) {
		float f = ((float) i / (float) n);

		vec4 c = Vec4(fmax(0.0f, sinf(f * 6.28f)),
						fmax(0.0f, sinf(f * 6.28f + 2.1f)),
						fmax(0.0f, sinf(f * 6.28f + 4.2f)), 1.0f);

		vec4Normalize(&c);
		vec4Mul(&c, 4.0f);

		r_drawLine(r, Vec4(0.0f, 0.0f, 0.0f, 0.0f), Vec4(cosf(f * M_PI * 2.0f), sinf(f * M_PI * 2.0f), 0.0f, 0.0f), c);
	}

	for (float x = -5.0f; x < 5.0f; x += 0.22f) {
		for (float y = -5.0f; y < 5.0f; y += 0.22f) {
			for (float z = -5.0f; z < 5.0f; z += 0.22f) {
				r_drawLine(r, Vec4(x, y, z, 0.0f), Vec4(x + 0.1f * sinf(y * 3.5f + r->ticks * 0.002f),
															y + 0.1f * sinf(x * 3.5f + r->ticks * 0.002f), z,
															0.0f), Vec4(0.0f, 4.0f, 0.0f, 1.0f));
			}
		}
	}

	r_render(r);
}

void r3(struct Renderer *r, struct Sys *s)
{
	//r_update(r);

	r->tex = &r->tex0;
	for (int i = 0; i < s->objNum; ++i) {
		int l = (int)(r->ticks / 1000.0f) % s->objs[i].volNum;
		//l = s->selected;
		for (int j = 0; j < l + 1; ++j) {
			for (int k = 0; k < 4; ++k) {
				int cur = l == j;
				vec4 v0 = Vec4(s->objs[i].volumes[j].faces[k]->verts[0],
								 s->objs[i].volumes[j].faces[k]->verts[1],
								 s->objs[i].volumes[j].faces[k]->verts[2], 0.0f);
			
				vec4 v1 = Vec4(s->objs[i].volumes[j].faces[k]->verts[4],
								 s->objs[i].volumes[j].faces[k]->verts[5],
								 s->objs[i].volumes[j].faces[k]->verts[6], 0.0f);
			
				vec4 v2 = Vec4(s->objs[i].volumes[j].faces[k]->verts[8],
								 s->objs[i].volumes[j].faces[k]->verts[9],
								 s->objs[i].volumes[j].faces[k]->verts[10], 0.0f);
			
				vec4 n = vec4Copy3(&s->objs[i].volumes[j].faces[k]->centroid);
				vec4 nn = vec4Copy3(&s->objs[i].volumes[j].faces[k]->normal);
				vec4Mul(&nn, 0.2f);
				vec4Add(&n, &nn);
			
				r_drawLine(r,
						   v0,
						   v1,
						   Vec4(10.0f, 10.0f * cur, 0.0f, 1.0f));
			
				r_drawLine(r,
						   v1,
						   v2,
						   Vec4(10.0f, 10.0f * cur, 0.0f, 1.0f));
			
				r_drawLine(r,
						   v2,
						   v0,
						   Vec4(10.0f, 10.0f * cur, 0.0f, 1.0f));
			
				/*r_drawLine(r,
				  s->objs[i].volumes[j].faces[k]->centroid,
				  n,
				  p_vec4(0.0f, 10.0f, 0.0f, 1.0f));*/
			}
		}
	}
	r_render(r);
}

void r4(struct Renderer *r, struct Sys *s)
{
	//r_update(r);

	r->tex = &r->tex0;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			for (int k = 0; k < 4; ++k) {
				vec4 n = vec4Copy3(&s->objs[i].volumes[j].faces[k]->centroid);
				vec4 nn = vec4Copy3(&s->objs[i].volumes[j].faces[k]->mFlux);

				float l = vec4Len(&nn);
				
				vec4Add(&nn, &n);
			
				r_drawLine(r,
						   n,
						   nn,
						   Vec4(10.0f * l, 10.0f * fmax(0.0f, 1.0f - l), 0.0f, 1.0f));
			}
		}
	}
	r_render(r);
}

void r_draw(struct Renderer *r, struct Sys *s)
{
	r1(r, s);
	//r4(r, s);
	r3(r, s);
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

	glLineWidth(4.0f);
	
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

	glLineWidth(4.0f);

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
