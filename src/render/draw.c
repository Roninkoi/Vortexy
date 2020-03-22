#include <util/util.h>
#include "render.h"

void rm(struct Renderer *r, struct Sys *s)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		/*p_meshTransform(&s->objs[i].mesh, &r->model);
		  p_meshTransform(&s->objs[i].mesh, &r->view);
		  p_meshTransform(&s->objs[i].mesh, &r->proj);*/
		p_meshSetCol(&s->objs[i].mesh, 0.0f, 0.08f, 0.10f, 0.10f);

		r_drawMesh(r, &s->objs[i].mesh);
	}

	//r_sort(r);
	r_render(r);
}

void rw(struct Renderer *r, struct Sys *s)
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		p_meshSetCol(&s->objs[i].mesh, 0.0f, 0.5f * 1.5f, 0.7f * 1.5f, 1.0f * 1.5f);
		r_drawWireMesh(r, &s->objs[i].mesh, 2.0f);
	}

	r_render(r);
}

void rv(struct Renderer *r, struct Sys *s)
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		int l = s->selected;
		r_drawVolume(r, &s->objs[i].volumes[l], Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	r_render(r);
}

void rf(struct Renderer *r, struct Sys *s)
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		int l = s->selected;
		r_drawFace(r, &s->objs[i].faces[l], Vec4(1.0f, (float) s->objs[i].faces[l].boundary, 0.0f, 1.0f));
		r_render(r);

		vec4 n = vec4Copy3(&s->objs[i].faces[l].centroid);
		vec4 nn = vec4Copy3(&s->objs[i].faces[l].surface);

		vec4 n0 = vec4Copy(&n);

		vec4Add(&nn, &n0);

		vec4 col = Vec4(1.0f, 0.0f, 1.0f, 1.0f);

		r_drawLine(r, n, nn, col, 2.0f);
		r_render(r);
	}
}

float vecscale = 0.2f;

void rl(struct Renderer *r, struct Sys *s)
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			vec4 n = vec4Copy3(&s->objs[i].volumes[j].centroid);
			vec4 nn = vec4Copy3(&s->objs[i].volumes[j].v);

			vec4Mul(&nn, vecscale);
			float l = vec4Len(&nn);

			if (l <= 0.0f)
				continue;

			vec4 n0 = vec4Copy(&n);

			vec4Add(&nn, &n0);

			vec4 col = Vec4(l, fmax(0.0f, 1.0f - l), 0.0f, 1.0f);
			vec4Mul(&col, 5.5f);

			r_drawLine(r, n, nn, col, 2.0f);
		}

	}
	r_render(r);
}

void rfl(struct Renderer *r, struct Sys *s)
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			vec4 n = vec4Copy3(&s->objs[i].faces[j].centroid);
			vec4 nn = vec4Copy3(&s->objs[i].faces[j].v);

			vec4Mul(&nn, vecscale);
			float l = vec4Len(&nn);

			if (l <= 0.0f)
				continue;

			vec4 n0 = vec4Copy(&n);

			vec4Add(&nn, &n0);

			vec4 col = Vec4(l, fmax(0.0f, 1.0f - l), 0.0f, 1.0f);
			vec4Mul(&col, 5.5f);

			r_drawLine(r, n, nn, col, 2.0f);
		}

	}
	r_render(r);
}

void rtl(struct Renderer *r, struct Sys *s)
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].volNum; ++j) {
			vec4 n = vec4Copy3(&s->objs[i].volumes[j].centroid);
			vec4 nn = vec4Copy3(&s->objs[i].volumes[j].v);

			vec4Mul(&nn, vecscale);
			
			float l = vec4Len(&nn);

			if (l <= 0.0f)
				continue;

			vec4Add(&nn, &n);

			vec4 col = Vec4(l, fmax(0.0f, 1.0f - l), 0.0f, 1.0f);
			vec4Mul(&col, 5.5f);

			r_drawVec(r, n, nn, col, r->camPos.z * 0.01f * log10(l + 1.0));
		}
	}
	r_render(r);
}

void rftl(struct Renderer *r, struct Sys *s)
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_DST_ALPHA, GL_SRC_ALPHA);

	r->tex = &r->flat;
	for (int i = 0; i < s->objNum; ++i) {
		for (int j = 0; j < s->objs[i].faceNum; ++j) {
			vec4 n = vec4Copy3(&s->objs[i].faces[j].centroid);
			vec4 nn = vec4Copy3(&s->objs[i].faces[j].v);

			vec4Mul(&nn, vecscale);
			
			float l = vec4Len(&nn);

			if (l <= 0.0f)
				continue;

			vec4Add(&nn, &n);

			vec4 col = Vec4(l, fmax(0.0f, 1.0f - l), 0.0f, 1.0f);
			vec4Mul(&col, 5.5f);

			r_drawVec(r, n, nn, col, r->camPos.z * 0.01f * log10(l + 1.0));
		}
	}
	r_render(r);
}

void r_draw(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	if (getBit(r->vis, 1))
		rm(r, s);
	if (getBit(r->vis, 2))
		rw(r, s);
	if (getBit(r->vis, 3))
		rl(r, s);
	if (getBit(r->vis, 4))
		rtl(r, s);

	if (getBit(r->vis, 5))
		rv(r, s);
	if (getBit(r->vis, 6))
		rf(r, s);
}

void r_rdraw(struct Renderer *r, struct Sys *s)
{
	r_update(r);

	if (getBit(r->vis, 1))
		rm(r, s);
	if (getBit(r->vis, 2))
		rw(r, s);
	
	if (getBit(r->vis, 3))
		rfl(r, s);
	if (getBit(r->vis, 4))
		rftl(r, s);
}

void r_drawMesh(struct Renderer *r, Mesh *m)
{
	r->drawMode = GL_TRIANGLES;

	r_add(r, m->vertData, m->texData, m->normData, m->colData, m->indData,
		  m->vertNum, m->indNum);
}

void r_drawFace(struct Renderer *r, struct Face *f, vec4 col)
{
	r->drawMode = GL_TRIANGLES;

	float vd[12];

	float td[12];

	float nd[12];

	float cd[12];

	int id[3] = {0, 1, 2};

	int k = 0;
	for (int j = 0; j < 12; ++j) {
		vd[k++] = f->verts[j];
	}

	for (int i = 0; i < 12; i += 1) {
		td[i] = 0.0f;
		nd[i] = 0.0f;
	}

	for (int i = 0; i < 3; i += 1) {
		cd[i * 4] = col.x;
		cd[i * 4 + 1] = col.y;
		cd[i * 4 + 2] = col.z;
		cd[i * 4 + 3] = col.w;
	}

	r_add(r, vd, td, nd, cd, id, 12, 3);
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

void r_drawVec(struct Renderer *r, vec4 v0, vec4 v1, vec4 col, float width)
{
	r->drawMode = GL_TRIANGLES;

	//float width = 0.02f * 10.0f;

	float a = atanf((v1.y - v0.y) / (v1.x - v0.x));
	float mr = r->modelRot.y;
	mr = 0.0f;

	float x = -width * sin(a) * cos(mr);
	float y = width * sin(a) * sin(mr) + width * cos(a) * cos(mr);
	float z = width * sin(a) * sin(mr);

	float vd[12] = {
					v0.x + x, v0.y + y, v0.z + z, v0.w,
					v0.x - x, v0.y - y, v0.z - z, v0.w,
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
