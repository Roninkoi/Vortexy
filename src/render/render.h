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
#include "phys/mat.h"
#include "phys/sys.h"
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

	GLuint modelUni;
	GLuint viewUni;
	GLuint projUni;

	GLuint texUni;

	GLenum drawMode;

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
	float ticks;
	float delta;

	int batches;
	int instances;

	int vertexNum;
	int indexNum;

	mat4 model;
	mat4 view;
	mat4 proj;

	vec4 camPos;
	vec4 camRot;
};

// RENDER
void r_init(struct Renderer *r, int *running);

void r_update(struct Renderer *r);

// render buffer contents
void r_render(struct Renderer *r);

// add data to buffers
void r_add(struct Renderer *r,
		   float *vd, float *td, float *nd, float *cd, int *id,
		   int vn, int in);

// copy data to gpu
void r_copy(struct Renderer *r);

// flush remaining data for drawing
void r_flush(struct Renderer *r);

// DRAW
void r_draw(struct Renderer *r, struct Sys *s);

void r_drawMesh(struct Renderer *r, Mesh *m);

void r_drawWireMesh(struct Renderer *r, Mesh *m);

void r_drawLine(struct Renderer *r, vec4 v0, vec4 v1, vec4 col);

#endif
