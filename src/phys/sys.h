#ifndef SYS_H
#define SYS_H

#include "phys/fluid.h"
#include "phys/obj.h"

// physical system
struct Sys {
	struct Fluid fluid;

	Obj *objs;

	int objNum;
};

void p_sysInit(struct Sys *s);

void p_sysTick(struct Sys *s);

#endif
