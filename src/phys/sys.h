#ifndef SYS_H
#define SYS_H

#include "fluid.h"
#include "obj.h"

// physical system
struct Sys {
	struct Fluid fluid;

	vec4 ext; // external field

	Obj *objs;

	int objNum;
	int ticks;

	int selected;
};

void p_sysInit(struct Sys *s);

void p_sysTick(struct Sys *s);

void p_sysStart(struct Sys *s);

void p_addObj(struct Sys *s, char *meshPath, char *fluidPath);

#endif
