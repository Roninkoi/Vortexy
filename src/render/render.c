#include "render.h"
#include "util/texLoader.h"

void r_init(struct Renderer *r, int *running)
{
	r->running = running;
	r->ticks = 0.0f;

	r->vertexNum = 0;
	r->indexNum = 0;
	r->batches = 0;
	r->draws = 0;

	r->width = SCREEN_WIDTH;
	r->height = SCREEN_HEIGHT;

	r->drawMode = GL_TRIANGLES;

	r->vis = 0b10010111;

	int glfwErr = glfwInit();
	if (!glfwErr) {
		printf("Failed to init GLFW %i%s", glfwErr, "\n");
	}

	r_createWindow(&r->window);

	glewExperimental = GL_TRUE;
	int glewErr = glewInit();
	if (glewErr != GLEW_OK) {
		printf("Failed to init GLEW %i%s", glewErr, "\n");
	}

	r_loadShader(&r->shader, VERT_PATH, FRAG_PATH);

	glGenVertexArrays(1, &r->vertexArray);
	glBindVertexArray(r->vertexArray);

	glGenBuffers(1, &r->vertBuffer);
	glGenBuffers(1, &r->texBuffer);
	glGenBuffers(1, &r->normBuffer);
	glGenBuffers(1, &r->colBuffer);
	glGenBuffers(1, &r->indBuffer);

	r->texUni = glGetUniformLocation(r->shader.program, "tex");

	r->modelUni = glGetUniformLocation(r->shader.program, "model");
	r->viewUni = glGetUniformLocation(r->shader.program, "view");
	r->projUni = glGetUniformLocation(r->shader.program, "proj");

	r->sUni = glGetUniformLocation(r->shader.program, "rs");

	r->model = imat4();
	r->view = imat4();
	r->proj = imat4();

	r->camPos = nvec4();
	r->camRot = nvec4();
	r->modelRot = nvec4();

	r_flatTex(&r->flat, 255, 255, 255, 128, 128);
	r->tex = &r->flat;
}

float *floatMat4(mat4 *m)
{
	float *a = calloc(16, sizeof(float));

	a[0] = (float) m->m[0][0];
	a[1] = (float) m->m[0][1];
	a[2] = (float) m->m[0][2];
	a[3] = (float) m->m[0][3];

	a[4] = (float) m->m[1][0];
	a[5] = (float) m->m[1][1];
	a[6] = (float) m->m[1][2];
	a[7] = (float) m->m[1][3];

	a[8] = (float) m->m[2][0];
	a[9] = (float) m->m[2][1];
	a[10] = (float) m->m[2][2];
	a[11] = (float) m->m[2][3];

	a[12] = (float) m->m[3][0];
	a[13] = (float) m->m[3][1];
	a[14] = (float) m->m[3][2];
	a[15] = (float) m->m[3][3];

	return a;
}

void r_update(struct Renderer *r)
{
	r->batches = 0;
	r->draws = 0;

	r->ticks += 1.0f;

	r_updateWindow(&r->window, r->running);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(r->clrCol.x, r->clrCol.y, r->clrCol.z, r->clrCol.w);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	r->width = r->window.width;
	r->height = r->window.height;
	glViewport(0, 0, r->width, r->height);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(r->texUni, 0);

	r->model = imat4();
	r->model = mat4RotateY(&r->model, r->modelRot.y);
	r->proj = mat4Perspective(1.5f, 1.78f, 0.1f, 100.0f);

	r->view = Mat4(1.0f);
	r->view = mat4RotateX(&r->view, r->camRot.x);
	r->view = mat4RotateY(&r->view, r->camRot.y);
	r->view = mat4Translate(&r->view, Vec4(-r->camPos.x, -r->camPos.y, -r->camPos.z, 0.0f));
	/*
	  mat4 faux = imat4();
	  glUniformMatrix4fv(r->modelUni, 1, GL_TRUE, &faux.m[0][0]);
	  glUniformMatrix4fv(r->projUni, 1, GL_TRUE, &faux.m[0][0]);
	  glUniformMatrix4fv(r->viewUni, 1, GL_TRUE, &faux.m[0][0]);*/
	/*
	  r->model = imat4();
	  r->view = imat4();
	  r->proj = imat4();*/

	float *ma = floatMat4(&r->model);
	float *pa = floatMat4(&r->proj);
	float *va = floatMat4(&r->view);

	glUniformMatrix4fv(r->modelUni, 1, GL_TRUE, ma);
	glUniformMatrix4fv(r->projUni, 1, GL_TRUE, pa);
	glUniformMatrix4fv(r->viewUni, 1, GL_TRUE, va);

	free(ma);
	free(pa);
	free(va);

	glUniform1f(r->sUni, r->rs);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
}

void r_add(struct Renderer *r,
		   float *vd, float *td, float *nd, float *cd, int *id,
		   int vn, int in)
{
	++r->draws;

	if (r->vertexNum + vn >= BATCH_SIZE || r->indexNum + in >= BATCH_SIZE) {
		return;
		r_render(r); // check later
	}

	for (int i = 0; i < vn; ++i) {
		r->vertices[r->vertexNum + i] = vd[i];
		r->texes[r->vertexNum + i] = td[i];
		r->normals[r->vertexNum + i] = nd[i];
		r->colors[r->vertexNum + i] = cd[i];
	}

	for (int i = 0; i < in; ++i) {
		r->indices[r->indexNum + i] = id[i] + r->indexNum;
	}

	r->vertexNum += vn;
	r->indexNum += in;
}

void r_copy(struct Renderer *r)
{
	glBindBuffer(GL_ARRAY_BUFFER, r->vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, r->vertexNum * sizeof(float), r->vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, r->texBuffer);
	glBufferData(GL_ARRAY_BUFFER, r->vertexNum * sizeof(float), r->texes, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, r->normBuffer);
	glBufferData(GL_ARRAY_BUFFER, r->vertexNum * sizeof(float), r->normals, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, r->colBuffer);
	glBufferData(GL_ARRAY_BUFFER, r->vertexNum * sizeof(float), r->colors, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->indBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, r->indexNum * sizeof(int), r->indices, GL_DYNAMIC_DRAW);
}

void r_flush(struct Renderer *r)
{
	++r->batches;

	glUseProgram(r->shader.program);

	glBindVertexArray(r->vertexArray);

	glBindAttribLocation(r->shader.program, 0, "pos");
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, r->vertBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindAttribLocation(r->shader.program, 1, "tex");
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, r->texBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindAttribLocation(r->shader.program, 2, "norm");
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, r->normBuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindAttribLocation(r->shader.program, 3, "col");
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, r->colBuffer);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->indBuffer);

	glDrawElements(r->drawMode, r->indexNum, GL_UNSIGNED_INT, (void *) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	r->vertexNum = 0;
	r->indexNum = 0;
}

float getCentZ(struct Renderer *r, int i0, int i1, int i2)
{
	float z = r->vertices[r->indices[i0] * 4 + 2] +
			  r->vertices[r->indices[i1] * 4 + 2] +
			  r->vertices[r->indices[i2] * 4 + 2];

	z /= 3.0f;

	return z;
}

void r_sort(struct Renderer *r)
{
	if (r->drawMode != GL_TRIANGLES) return;

	int n = r->indexNum / 3;

	for (int i = 0; i < n; i += 3) {
		int imax = i;
		float vmax = getCentZ(r, i, i + 1, i + 2);

		for (int j = i + 1; j < n; j += 3) {
			float tmax = getCentZ(r, j, j + 1, j + 2);

			if (tmax > vmax) {
				imax = j;
				vmax = tmax;
			}
		}

		int i0 = r->indices[i];
		int i1 = r->indices[i + 1];
		int i2 = r->indices[i + 2];

		r->indices[i] = r->indices[imax];
		r->indices[i + 1] = r->indices[imax + 1];
		r->indices[i + 2] = r->indices[imax + 2];

		r->indices[imax] = i0;
		r->indices[imax + 1] = i1;
		r->indices[imax + 2] = i2;
	}
}

void r_render(struct Renderer *r)
{
	glBindTexture(GL_TEXTURE_2D, r->tex->tex);

	r_copy(r);

	r_flush(r);
}
