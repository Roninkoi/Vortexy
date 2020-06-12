/*
  Vortexy - Fluid dynamics simulator
  Roni Koitermaa 2019
*/

#ifndef SIM_H
#define SIM_H

#include <stdio.h>
#include <stdlib.h>

#include "phys/sys.h"
#include "util/randomUtil.h"

// enable rendering
#define RENDER_ENABLED 1

// renderer include
#if RENDER_ENABLED

#include "render/render.h"
#include "render/input.h"

#endif

struct Sim {
#if RENDER_ENABLED
	struct Renderer renderer;
#endif

	struct Sys sys;

	int ticks;
	int running;

	int tps; // ticks per second
	int ticksOld;
	unsigned int time; // real time
	unsigned int timeOld;
	unsigned int startTime;

	unsigned int st;
	
	float rz;
	float rs;
	float ps;
	float vs;
	vec3 bgcol;
	int rmode;

	int autoquit;

	FILE *file;
	int outputt;
	int outputting;
	int outputf;
	int inputf;

	int inputram;
	int inputi;
	char **inputdat;

	int rendered;

	int mode;

	char *fluidPath;
	char *filePath;
};

void s_init(struct Sim *s);

void s_run(struct Sim *s);

void s_tick(struct Sim *s);

#endif
