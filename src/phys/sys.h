#ifndef SYS_H
#define SYS_H

#include "fluid.h"
#include "obj.h"
#include "solver.h"

// physical system
struct Sys {
	Obj *objs;

	int objNum;
	int ticks;
	float time;

	float t;
	float dt;

	int selected;
};

void p_sysInit(struct Sys *s);

void p_sysTick(struct Sys *s);

void p_sysStart(struct Sys *s);

void p_addObj(struct Sys *s, char *meshPath, char *fluidPath);

#endif
