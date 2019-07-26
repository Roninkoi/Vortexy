#ifndef VEC_H
#define VEC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// four-vector type
typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4;

vec4 p_vec4(float x, float y, float z, float w);

void p_vec4Init(vec4 *v, float x, float y, float z, float w);

void p_vec4Print(vec4 *v);

vec4 p_ivec4();

vec4 p_nvec4();

void p_vec4Add(vec4 *v0, vec4 *v1);

void p_vec4Sub(vec4 *v0, vec4 *v1);

void p_vec4Mul(vec4 *v, float s);

void p_vec4Div(vec4 *v, float s);

float p_vec4Len(vec4 *v);

vec4 p_vec4Cross(vec4 *v0, vec4 *v1);

float p_vec4Dot(vec4 *v0, vec4 *v1);

#endif
