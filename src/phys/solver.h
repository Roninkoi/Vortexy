#ifndef SOLVER_H
#define SOLVER_H

#include "volume.h"
#include "sys.h"

// volume upwind from face
struct Volume *getUpwindVol(struct Face *f);

// face upwind from volume
struct Face *getUpwindFace(struct Volume *v);

// get velocity field from matrix
void p_getV(Obj *o);

// get pressure from matrix
void p_getP(Obj *o);

// construct pressure equation matrix
void p_constructPMat(Obj *o);

void p_faceD(struct Face *f);

void p_D(Obj *o);

// calculate coefficients for pressure equation
void p_computePCoeffs(Obj *o);

// construct matrix for momentum equation
void p_constructVMat(Obj *o);

// calculate coefficients for momentum equation
void p_computeVCoeffs(Obj *o);

// compute face fluxes
void p_computeFaceFs(Obj *o);

// compute volume fluxes
void p_computeVolFs(Obj *o);

// minimum correction decomposition of surfaces
void p_decomposeSurfs(Obj *o);

// interpolate face velocity from volumes
void p_faceVI(struct Face *f);

// interpolate face pressure from volumes
void p_facePI(struct Face *f);

void p_faceP(struct Face *f);

// velocity correction
void p_faceVC(struct Face *f);

void p_faceV(struct Face *f);

// interpolated face pressure gradient
void p_pFaceGradI(Obj *o);

// interpolated face velocity gradient
void p_vFaceGradI(Obj *o);

void p_pFaceGrad(Obj *o);

void p_vFaceGrad(Obj *o);

// volume pressure gradient
void p_pGrad(Obj *o);

// volume velocity gradient
void p_vGrad(Obj *o);

#endif
