/*
  Vortexy - GPU fluid dynamics simulator
  Roni Koitermaa 2019
*/

#ifndef SIM_H
#define SIM_H

#include <stdio.h>
#include <stdlib.h>

#include "phys/sys.h"
#include "util/randomUtil.h"

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

// OpenCL
#include <CL/cl.h>

// enable rendering
#define RENDER_ENABLED 1

// kernel size
#define SRC_MAX 0x100000

// renderer include
#if RENDER_ENABLED == 1

#include "render/render.h"
#include "render/input.h"

#endif

struct Sim {
#if RENDER_ENABLED == 1
	struct Renderer renderer;
#endif

	struct Sys sys;

	int ticks;
	int running;

	int tps; // ticks per second
	int ticksOld;
	unsigned int timeOld;
	unsigned int tickTime;

	int rendered;
	int usegpu;

	double time; // simulated time
	float dt; // time step
};

void s_init(struct Sim *s);

void s_run(struct Sim *s);

void s_tick(struct Sim *s);

#endif
