#ifndef FLUID_H
#define FLUID_H

#include "util/vec.h"

struct Fluid {	
	float rho; // density
	float nu; // kinematic viscosity
	float mu; // dynamic viscosity

	float re; // reynolds number

	float bp; // base pressure
	
	vec3 f; // source

	int ebc; // edge boundary condition
};

void p_loadFluid(struct Fluid *f, char *path);

#endif
