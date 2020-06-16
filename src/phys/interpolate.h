//
// Created by rak on 6/4/20.
//

#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include "volume.h"
#include "sys.h"

unsigned int halfGWF = 0; // halfway approximation for interpolation
unsigned int gradIt = 2; // number of gradient iterations
unsigned int transient = 1; // transient solution?

// calculate geometric weighting factor
real getGWF(struct Face *f);
real getGWFh(struct Face *f);
real getGWFp(struct Face *f);

// interpolate face C from volumes
void p_faceC(struct Face *f);

// interpolate face velocity from volumes
void p_faceVI(struct Face *f);

// interpolate face pressure from volumes
void p_facePI(struct Face *f);

void p_faceP(struct Face *f);

void p_faceBoundP(Obj *o);

void p_faceV(struct Face *f, int rc);
void p_faceVE(struct Face *f, real urf);

void p_facePC(struct Face *f);

// velocity correction
void p_faceVRC(struct Face *f);

void p_faceVRCE(struct Face *f, real urf);

#endif
