#include "render/render.h"

void r_init(struct Renderer *r, int *running)
{
	r->running = running;

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
}

void r_update(struct Renderer *r)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void r_render(struct Renderer *r)
{
	r_update(r);

	r_updateWindow(&r->window, r->running);
}
