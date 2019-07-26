/*
  Vortexy renderer
  Roni Koitermaa 2019
*/

#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "render/window.h"
#include "phys/mesh.h"
#include "texture.h"
#include "shader.h"

#define VERT_PATH "src/render/shader/main.vert"
#define FRAG_PATH "src/render/shader/main.frag"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define BATCH_SIZE 16384

struct Renderer {
	struct Window window;

	GLuint vertexArray;

	GLuint vertBuffer;
	GLuint texBuffer;
	GLuint normBuffer;
	GLuint colBuffer;
	GLuint indBuffer;

	float vertices[BATCH_SIZE];
	float texes[BATCH_SIZE];
	float normals[BATCH_SIZE];
	float colors[BATCH_SIZE];
	int indices[BATCH_SIZE];

	Texture tex;

	GLuint width;
	GLuint height;

	struct Shader shader;

	int *running;
};

// render
void r_render(struct Renderer *r);

void r_update(struct Renderer *r);

void r_init(struct Renderer *r, int *running);

// draw
void r_drawMesh(struct Mesh *m);

#endif
