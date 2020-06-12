#ifndef SYS_H
#define SYS_H

#include "fluid.h"
#include "obj.h"
#include "solver.h"

unsigned int simkill; // quit simulation (gracefully)

// physical system
struct Sys {
	Obj *objs;

	int objNum;
	int ticks;

	real endt;

	int dtMaxIt; // iterations within time-step
	real residual; // residual target

	int maxIt; // solver iterations
	real epsilon;

	int gradIt; // gradient iterations

	real relaxm;

	real res; // current residual
	int in; // iteration number

	int printitn;

	int divhalt;

	int reset;
	int selected;
	int simulating;

	int debugFlag;
	unsigned int unreal;
};

void p_sysInit(struct Sys *s)
{
	s->objs = NULL;
	s->objNum = 0;
	s->ticks = 0;

	s->endt = 1.0;

	s->dtMaxIt = 10;
	s->residual = 1.0e1;

	s->maxIt = 1000;
	s->epsilon = 1.0e-3;

	s->gradIt = 2;

	s->relaxm = 1.0;

	s->res = 0.0;
	s->in = 0;

	s->printitn = 1;

	s->divhalt = 0;

	s->reset = 0;
	s->selected = 0;
	s->simulating = 1;

	s->debugFlag = 0;
	s->unreal = 0;
}

void p_sysTick(struct Sys *s);

void p_sysStart(struct Sys *s);

void p_sysRestart(struct Sys *s, char *fluidPath);

void p_sysEnd(struct Sys *s);

void p_addObj(struct Sys *s, char *fluidPath, int mode);

void p_sysStatus(struct Sys *s);

#endif
