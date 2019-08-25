#ifndef VEC_H
#define VEC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// three-vector type
typedef struct {
	float x;
	float y;
	float z;
} vec3;

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

float p_vec4Len3(vec4 *v);

float p_vec4Len(vec4 *v);

vec4 p_vec4Cross(vec4 *v0, vec4 *v1);

float p_vec4Dot(vec4 *v0, vec4 *v1);

int p_vec4Equ3(vec4 *v0, vec4 *v1);

int p_vec4Equ(vec4 *v0, vec4 *v1);

vec4 p_vec4Copy(vec4 *v);

vec4 p_vec4Copy3(vec3 *v);

/*
  VEC3
*/

vec3 p_vec3(float x, float y, float z);

void p_vec3Init(vec3 *v, float x, float y, float z);

void p_vec3Print(vec3 *v);

vec3 p_ivec3();

vec3 p_nvec3();

void p_vec3Add(vec3 *v0, vec3 *v1);

void p_vec3Sub(vec3 *v0, vec3 *v1);

void p_vec3Mul(vec3 *v, float s);

void p_vec3Div(vec3 *v, float s);

void p_vec3Normalize(vec3 *v);

float p_vec3Len(vec3 *v);

vec3 p_vec3Cross(vec3 *v0, vec3 *v1);

float p_vec3Dot(vec3 *v0, vec3 *v1);

vec3 p_vec3Copy(vec3 *v);

#endif
