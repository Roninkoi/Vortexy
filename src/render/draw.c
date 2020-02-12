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
	//r_update(r);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {/*
		p_meshTransform(&s->objs[i].mesh, &r->model);
		p_meshTransform(&s->objs[i].mesh, &r->view);
		p_meshTransform(&s->objs[i].mesh, &r->proj);*/

		r_drawMesh(r, &s->objs[i].mesh);
	}
	//r_sort(r);
	r_render(r);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	for (int i = 0; i < s->objNum; ++i) {
		p_meshSetCol(&s->objs[i].mesh, 0.0f, 0.5f, 0.7f, 1.0f);
		r_drawWireMesh(r, &s->objs[i].mesh, 2.0f);
	}

	r_render(r);
}

void r2(struct Renderer *r, struct Sys *s)
{
	//r_update(r);

	r->tex = &r->flat;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);
	int n = 400;
	for (int i = 0; i < n; ++i) {
		float f = ((float) i / (float) n);

		vec4 c = Vec4(fmax(0.0f, sinf(f * 6.28f)),
					  fmax(0.0f, sinf(f * 6.28f + 2.1f)),
					  fmax(0.0f, sinf(f * 6.28f + 4.2f)), 1.0f);

		vec4Normalize(&c);
		vec4Mul(&c, 4.0f);

		r_drawLine(r, Vec4(0.0f, 0.0f, 0.0f, 0.0f), Vec4(cosf(f * M_PI * 2.0f), sinf(f * M_PI * 2.0f), 0.0f, 0.0f), c, 2.0f);
	}

	for (float x = -5.0f; x < 5.0f; x += 0.22f) {
		for (float y = -5.0f; y < 5.0f; y += 0.22f) {
			for (float z = -5.0f; z < 5.0f; z += 0.22f) {
				r_drawLine(r, Vec4(x, y, z, 0.0f), Vec4(x + 0.1f * sinf(y * 3.5f + r->ticks * 0.002f),
														y + 0.1f * sinf(x * 3.5f + r->ticks * 0.002f), z,
														0.0f), Vec4(0.0f, 4.0f, 0.0f, 1.0f), 2.0f);
			}
		}
	}

	r_render(r);
}

void r3(struct Renderer *r, struct Sys *s)
{
	//r_update(r);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);

	r->tex = &r->tex0;
	for (int i = 0; i < s->objNum; ++i) {
		int l = (int) (r->ticks / 100.0f) % s->objs[i].volNum;
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

				/*r_drawLine(r,
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
						   Vec4(10.0f, 10.0f * cur, 0.0f, 1.0f));*/

				/*r_drawLine(r,
				  s->objs[i].volumes[j].faces[k]->centroid,
				  n,
				  p_vec4(0.0f, 10.0f, 0.0f, 1.0f));*/
				r_drawVolume(r, &s->objs[i].volumes[j], Vec4(1.0f, 0.0f, 0.0f, 1.0f));
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

				if (l <= 0.0f)
					continue;

				vec4Add(&nn, &n);

				r_drawLine(r,
						   n,
						   nn,
						   Vec4(10.0f * l, 10.0f * fmax(0.0f, 1.0f - l), 0.0f, 1.0f), 2.0f);
			}
		}
	}
	r_render(r);
}

float vecscale = 0.2f;

void r5(struct Renderer *r, struct Sys *s)
{
	//r_update(r);

	r->tex = &r->tex0;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			vec4 n = vec4Copy3(&s->objs[i].volumes[j].centroid);
			vec4 nn = vec4Copy3(&s->objs[i].volumes[j].mFlux);

			vec4Mul(&nn, vecscale);
			float l = vec4Len(&nn);

			if (l <= 0.0f)
				continue;

			vec4 n0 = vec4Copy(&n);

			vec4 noffs = vec4Copy(&nn);
			vec4Normalize(&noffs);
			vec4Mul(&noffs, 0.005f);
			vec4Add(&n, &noffs);

			vec4Add(&nn, &n0);

			r_drawLine(r,
					   n,
					   nn,
					   Vec4(10.0f * l, 10.0f * fmax(0.0f, 1.0f - l), 0.0f, 1.0f), 2.0f);
		}

	}
	r_render(r);
}

void r6(struct Renderer *r, struct Sys *s)
{
	//r_update(r);

	r->tex = &r->tex0;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			vec4 n = vec4Copy3(&s->objs[i].volumes[j].centroid);
			vec4 nn = vec4Copy3(&s->objs[i].volumes[j].mFlux);

			vec4Mul(&nn, vecscale);
			float l = vec4Len(&nn);

			if (l <= 0.0f)
				continue;

			vec4Add(&nn, &n);

			r_drawVec(r,
					  n,
					  nn,
					  Vec4(10.0f * l, 10.0f * fmax(0.0f, 1.0f - l), 0.0f, 1.0f));
		}
	}
	r_render(r);
}

void r_draw(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	r1(r, s);
	r5(r, s);
	r6(r, s);

	//r3(r, s);
}

void r_drawMesh(struct Renderer *r, Mesh *m)
{
	r->drawMode = GL_TRIANGLES;

	r_add(r, m->vertData, m->texData, m->normData, m->colData, m->indData,
		  m->vertNum, m->indNum);
}

void r_drawVolume(struct Renderer *r, struct Volume *v, vec4 col)
{
	r->drawMode = GL_TRIANGLES;

	float vd[48];

	float td[48];

	float nd[48];

	float cd[48];

	int id[12] = {0, 1, 2,
				  3, 4, 5,
				  6, 7, 8,
				  9, 10, 11};

	int k = 0;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 12; ++j) {
			vd[k++] = v->faces[i]->verts[j];
		}
	}

	for (int i = 0; i < 48; i += 1) {
		td[i] = 0.0f;
		nd[i] = 0.0f;
	}

	for (int i = 0; i < 12; i += 1) {
		cd[i * 4] = col.x;
		cd[i * 4 + 1] = col.y;
		cd[i * 4 + 2] = col.z;
		cd[i * 4 + 3] = col.w;
	}

	r_add(r, vd, td, nd, cd, id, 48, 12);
}

void r_drawWireMesh(struct Renderer *r, Mesh *m, float thickness)
{
	r->drawMode = GL_LINES;

	glLineWidth(thickness);

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

void r_drawLine(struct Renderer *r, vec4 v0, vec4 v1, vec4 col, float thickness)
{
	r->drawMode = GL_LINES;

	glLineWidth(thickness);

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

void r_drawVec(struct Renderer *r, vec4 v0, vec4 v1, vec4 col)
{
	r->drawMode = GL_TRIANGLES;

	float width = 0.015f;

	float vd[12] = {
			v0.x, v0.y + width, v0.z, v0.w,
			v0.x, v0.y - width, v0.z, v0.w,
			v1.x, v1.y, v1.z, v1.w
	};

	float td[12] = {
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
	};

	float nd[12] = {
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
	};

	float cd[12] = {
			col.x, col.y, col.z, col.w,
			col.x, col.y, col.z, col.w,
			col.x, col.y, col.z, col.w
	};

	int id[3] = {0, 1, 2};

	r_add(r, vd, td, nd, cd, id, 12, 3);
}
