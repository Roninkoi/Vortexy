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

	GLFWimage icons[1];
	icons[0].pixels = ppmLoaderAlpha("gfx/vortexyicon.ppm", &icons[0].width, &icons[0].height);
	glfwSetWindowIcon(r->window.window, 1, icons);
	free(icons[0].pixels);

	r->model = p_imat4();
	r->view = p_imat4();
	r->proj = p_imat4();

	r->camPos = p_nvec4();
	r->camRot = p_nvec4();

	r->camPos.z = -2.0f;

	r_flatTex(&r->flat, 255, 255, 255, 128, 128);
	r->tex = &r->flat;

	r_loadTex(&r->tex0, "data/test.ppm");
}

void r_update(struct Renderer *r)
{
	r->batches = 0;
	r->draws = 0;

	r->ticks += 1.0f;

	r_updateWindow(&r->window, r->running);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	r->width = r->window.width;
	r->height = r->window.height;
	glViewport(0, 0, r->width, r->height);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(r->texUni, 0);

	//r->model = p_mat4RotateY(&r->model, 0.01f);
	r->proj = p_mat4Perspective(1.5f, 1.78f, 0.1f, 100.0f);

	r->view = p_mat4(1.0f);
	r->view = p_mat4RotateX(&r->view, r->camRot.x);
	r->view = p_mat4RotateY(&r->view, r->camRot.y);
	r->view = p_mat4Translate(&r->view, p_vec4(-r->camPos.x, -r->camPos.y, -r->camPos.z, 0.0f));

	glUniformMatrix4fv(r->modelUni, 1, GL_TRUE, &r->model.m[0][0]);
	glUniformMatrix4fv(r->projUni, 1, GL_TRUE, &r->proj.m[0][0]);
	glUniformMatrix4fv(r->viewUni, 1, GL_TRUE, &r->view.m[0][0]);

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

void r_sort(struct Renderer *r)
{
	// TODO: sort by triangle centroid
}

void r_render(struct Renderer *r)
{
	glBindTexture(GL_TEXTURE_2D, r->tex->tex);
	
	r_copy(r);

	r_flush(r);
}
