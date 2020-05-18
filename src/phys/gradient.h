//
// Created by rak on 4/10/20.
//

#ifndef GRADIENT_H
#define GRADIENT_H

#include "volume.h"
#include "sys.h"

// interpolate face D from volumes
void p_faceD(struct Face *f);

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

// volume pressure correction gradient
void p_pcGrad(Obj *o);

// volume pressure gradient
void p_pGrads(Obj *o);

// volume velocity gradient
void p_vGrads(Obj *o);

// volume pressure correction gradient
void p_pcGrads(Obj *o);

// volume pressure gradient halfway
void p_pGradsh(Obj *o);

// volume velocity gradient halfway
void p_vGradsh(Obj *o);

// volume pressure correction gradient halfway
void p_pcGradsh(Obj *o);

#endif //GRADIENT_H
