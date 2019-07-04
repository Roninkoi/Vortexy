#ifndef SYS_H
#define SYS_H

#include "phys/fluid.h"
#include "phys/obj.h"

// physical system
struct Sys {
  struct Fluid fluid;

  struct Obj *objs;
};

void p_sysInit(struct Sys *s);

#endif
