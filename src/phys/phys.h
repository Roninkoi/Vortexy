#ifndef PHYS_H
#define PHYS_H

#include "util/vec.h"

// physics object
struct Phys {
	vec4 r; // position
	vec4 v; // velocity
	vec4 a; // acceleration
	vec4 f; // force

	float p; // pressure

	float m; // mass
	float rho; // density
	
	float T; // temperature
};

void p_physInit(struct Phys* p);

#endif
