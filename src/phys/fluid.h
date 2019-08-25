#ifndef FLUID_H
#define FLUID_H

struct Fluid {	
	float rho; // density
	float nu; // kinematic viscosity
	float mu; // dynamic viscosity
};

void p_loadFluid(struct Fluid *f, char *path);

#endif
