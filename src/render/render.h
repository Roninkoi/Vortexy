/*
  Vortexy renderer
  Roni Koitermaa 2019
*/

#ifndef RENDER_H
#define RENDER_H

#include "render/window.h"
#include "phys/mesh.h"

struct Renderer {
	struct Window window;
};

// render
void r_render(struct Renderer *r);

void r_init(struct Renderer *r);

// draw
void r_drawMesh(struct Mesh *m);

#endif
