#ifndef PHYS_H
#define PHYS_H

#include "util/vec.h"

// physics object
struct Phys {
	vec3 r; // position
	vec3 v; // velocity
	vec3 a; // acceleration
	vec3 f; // force

	float p; // pressure

	float m; // mass
	float rho; // density
	
	float T; // temperature
};

void p_physInit(struct Phys* p);

#endif
