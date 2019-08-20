#ifndef VEC_H
#define VEC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// general vector
typedef struct {
	float *v;
	int n; // dimension
} vec;

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

// one, vec4(1)
vec4 p_ivec4();

// null, vec4(0)
vec4 p_nvec4();

void p_vec4Add(vec4 *v0, vec4 *v1);

void p_vec4Sub(vec4 *v0, vec4 *v1);

void p_vec4Mul(vec4 *v, float s);

void p_vec4Div(vec4 *v, float s);

// scale to unit length
void p_vec4Normalize(vec4 *v);

float p_vec3Len(vec4 *v);

float p_vec4Len(vec4 *v);

vec4 p_vec4Cross(vec4 *v0, vec4 *v1);

float p_vec4Dot(vec4 *v0, vec4 *v1);

int p_vec3Equ(vec4 *v0, vec4 *v1);

int p_vec4Equ(vec4 *v0, vec4 *v1);

#endif
