/*
  Vortexy renderer
  Roni Koitermaa 2019
*/

#ifndef RENDER_H
#define RENDER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include "window.h"
#include "texture.h"
#include "shader.h"
#include "phys/mesh.h"
#include "phys/sys.h"
#include "util/mat.h"

#define VERT_PATH "src/render/shader/main.vert"
#define FRAG_PATH "src/render/shader/main.frag"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

// 16384
#define BATCH_SIZE 16384*16

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
	
	GLuint sUni;

	GLuint texUni;

	GLenum drawMode;

	float vertices[BATCH_SIZE];
	float texes[BATCH_SIZE];
	float normals[BATCH_SIZE];
	float colors[BATCH_SIZE];
	int indices[BATCH_SIZE];

	Texture *tex;

	Texture tex0;
	Texture tex1;

	Texture flat;

	GLuint width;
	GLuint height;

	struct Shader shader;

	int *running;
	float ticks;
	float delta;

	int batches;
	int draws;

	int vertexNum;
	int indexNum;

	mat4 model;
	mat4 view;
	mat4 proj;

	vec4 camPos;
	vec4 camRot;

	vec4 modelRot;

	float s;

	int vis;
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

// sort triangles for blending
void r_sort(struct Renderer *r);

// copy data to gpu
void r_copy(struct Renderer *r);

// flush remaining data for drawing
void r_flush(struct Renderer *r);

// DRAW
void r_draw(struct Renderer *r, struct Sys *s);
void r_rdraw(struct Renderer *r, struct Sys *s);

void r_drawMesh(struct Renderer *r, Mesh *m);

void r_drawWireMesh(struct Renderer *r, Mesh *m, float thickness);

void r_drawLine(struct Renderer *r, vec4 v0, vec4 v1, vec4 col, float thickness);

void r_drawVec(struct Renderer *r, vec4 v0, vec4 v1, vec4 col, float width);

void r_drawVolume(struct Renderer *r, struct Volume *v, vec4 col);

void r_drawFace(struct Renderer *r, struct Face *f, vec4 col);

#endif
