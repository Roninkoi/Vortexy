#ifndef FLUID_H
#define FLUID_H

#include "util/vec.h"

struct Fluid {	
	real rho; // density
	real nu; // kinematic viscosity
	real mu; // dynamic viscosity

	real re; // reynolds number

	real bp; // base pressure
	
	vec3 f; // source

	int ebc; // edge boundary condition
};

#endif
