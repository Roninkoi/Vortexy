//
// Created by rak on 4/10/20.
//

#ifndef GRADIENT_H
#define GRADIENT_H

#include "interpolate.h"

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
