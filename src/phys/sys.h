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

	float endt;

	int maxIt;
	float epsilon;

	int reset;
	int selected;
	int simulating;
};

void p_sysInit(struct Sys *s);

void p_sysTick(struct Sys *s);

void p_sysStart(struct Sys *s);

void p_sysRestart(struct Sys *s, char *fluidPath);

void p_sysEnd(struct Sys *s);

void p_addObj(struct Sys *s, char *fluidPath, int mode);

#endif
