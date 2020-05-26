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

	real endt;

	int dtMaxIt;
	int maxIt;
	real epsilon;
	int relaxm;
	real residual; // residual target
	real res; // current residual
	int in; // iteration number

	int printitn;

	int reset;
	int selected;
	int simulating;

	int debugFlag;
	int unreal;
};

void p_sysInit(struct Sys *s)
{
	s->objs = NULL;
	s->objNum = 0;
	s->simulating = 1;
	s->reset = 0;
	s->ticks = 0;
	s->selected = 0;
	s->debugFlag = 0;
	s->unreal = 0;
	s->residual = 0.0;
	s->res = 0.0;
	s->in = 0;
}

void p_sysTick(struct Sys *s);

void p_sysStart(struct Sys *s);

void p_sysRestart(struct Sys *s, char *fluidPath);

void p_sysEnd(struct Sys *s);

void p_addObj(struct Sys *s, char *fluidPath, int mode);

#endif
