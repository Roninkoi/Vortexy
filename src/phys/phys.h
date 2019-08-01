#ifndef PHYS_H
#define PHYS_H

#include "vec.h"

// physics object
struct Phys {
	vec4 r; // position
	vec4 v; // velocity
	vec4 a; // acceleration
	vec4 f; // force

	float m; // mass
};

void p_physInit(struct Phys* p);

#endif
