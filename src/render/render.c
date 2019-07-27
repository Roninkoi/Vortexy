#include "render/render.h"

void r_init(struct Renderer *r, int *running)
{
	r->running = running;
	r->ticks = 0.0f;

	r->width = SCREEN_WIDTH;
	r->height = SCREEN_HEIGHT;

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

	r->model = p_imat4();
	r->view = p_imat4();
	r->proj = p_imat4();

	r_loadTex(&r->tex, "data/test.ppm");
}

void r_update(struct Renderer *r)
{
	r->ticks += 1.0f;

	r_updateWindow(&r->window, r->running);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	r->width = r->window.width;
	r->height = r->window.height;
	glViewport(0, 0, r->width, r->height);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, r->tex.tex);
	glUniform1i(r->texUni, 0);

	r->model = p_mat4RotateY(&r->model, 0.01f);

	glUniformMatrix4fv(r->modelUni, 1, GL_FALSE, &r->model.m[0][0]);
	glUniformMatrix4fv(r->projUni, 1, GL_FALSE, &r->proj.m[0][0]);
	glUniformMatrix4fv(r->viewUni, 1, GL_FALSE, &r->view.m[0][0]);
}

void r_add(struct Renderer *r)
{

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

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);

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

	glDrawElements(GL_TRIANGLES, r->indexNum, GL_UNSIGNED_INT, (void *) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	r->vertexNum = 0;
	r->indexNum = 0;
}
