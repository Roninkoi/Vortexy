#ifndef SYS_H
#define SYS_H

#include "fluid.h"
#include "obj.h"

// physical system
struct Sys {
	struct Fluid fluid;

	Obj *objs;

	int objNum;
	int ticks;
};

void p_sysInit(struct Sys *s);

void p_sysTick(struct Sys *s);

#endif
