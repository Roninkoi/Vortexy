#ifndef SOLVER_H
#define SOLVER_H

#include "volume.h"
#include "sys.h"
#include "gradient.h"

// get velocity field from matrix
void p_getVX(Obj *o);
void p_getVY(Obj *o);
void p_getVZ(Obj *o);

// get pressure from matrix
void p_getP(Obj *o);

// construct pressure equation matrix
void p_constructPMat(Obj *o);

void p_computeC(Obj *o);

void p_computePBoundCoeffs(Obj *o);

// calculate coefficients for pressure equation
void p_computePCoeffs(Obj *o);

// construct matrix for momentum equation
void p_constructVMatX(Obj *o);
void p_constructVMatY(Obj *o);
void p_constructVMatZ(Obj *o);

void p_computeVBoundCoeffs(Obj *o);

// calculate coefficients for momentum equation
void p_computeVCoeffs(Obj *o);

void p_computeVFaceCoeffs(Obj *o);

// compute face fluxes
void p_computeFaceFs(Obj *o, int in, int tn);

// compute volume fluxes
void p_computeVolFs(Obj *o);

// minimum correction decomposition of surfaces
void p_decomposeSurfs(Obj *o);

// calculate face mass flow rate for volume
real p_getMrate(struct Volume *v, struct Face *f, real rho);

#endif
