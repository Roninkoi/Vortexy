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

#define OPENCL_ENABLED 0

#if OPENCL_ENABLED == 1

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

// kernel size
#define KERNEL_MAX 0x100000

// OpenCL
#include <CL/cl.h>

#endif

// enable rendering
#define RENDER_ENABLED 1

// renderer include
#if RENDER_ENABLED == 1

#include "render/render.h"
#include "render/input.h"

#endif

struct Sim {
#if OPENCL_ENABLED == 1
#endif
	
#if RENDER_ENABLED == 1
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

	FILE *out;
	int outputting;

	int rendered;
	int usegpu;
	int device;

	int mode;

	char *fluidPath;
	char *meshPath;
	char *outPath;
};

void s_init(struct Sim *s);

void s_run(struct Sim *s);

void s_tick(struct Sim *s);

#endif
