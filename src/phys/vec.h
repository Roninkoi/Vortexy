#ifndef VEC_H
#define VEC_H

#include <stdio.h>
#include <stdlib.h>

// internal phys four-vector type
typedef struct {
  float x;
  float y;
  float z;
  float w;
} p_vec4;

p_vec4 p_vec4Make(float x, float y, float z, float w);
void p_vec4Init(p_vec4 *v, float x, float y, float z, float w);
void p_vec4Print(p_vec4 *v);

#endif
