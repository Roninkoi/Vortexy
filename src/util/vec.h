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

vec4 Vec4(float x, float y, float z, float w);

void vec4Init(vec4 *v, float x, float y, float z, float w);

void vec4Print(vec4 *v);

// one, vec4(1)
vec4 ivec4();

// null, vec4(0)
vec4 nvec4();

void vec4Add(vec4 *v0, vec4 *v1);

void vec4Sub(vec4 *v0, vec4 *v1);

void vec4Mul(vec4 *v, float s);

void vec4Div(vec4 *v, float s);

// scale to unit length
void vec4Normalize(vec4 *v);

float vec4Len3(vec4 *v);

float vec4Len(vec4 *v);

vec4 vec4Cross(vec4 *v0, vec4 *v1);

float vec4Dot(vec4 *v0, vec4 *v1);

int vec4Equ3(vec4 *v0, vec4 *v1);

int vec4Equ(vec4 *v0, vec4 *v1);

vec4 vec4Copy(vec4 *v);

vec4 vec4Copy3(vec3 *v);

/*
  VEC3
*/

vec3 Vec3(float x, float y, float z);

void vec3Init(vec3 *v, float x, float y, float z);

void vec3Print(vec3 *v);

vec3 ivec3();

vec3 nvec3();

void vec3Add(vec3 *v0, vec3 *v1);

void vec3Sub(vec3 *v0, vec3 *v1);

void vec3Mul(vec3 *v, float s);

void vec3Div(vec3 *v, float s);

void vec3Normalize(vec3 *v);

float vec3Len(vec3 *v);

vec3 vec3Cross(vec3 *v0, vec3 *v1);

float vec3Dot(vec3 *v0, vec3 *v1);

vec3 vec3Copy(vec3 *v);

// return outwards pointing vector
vec3 vec3Outwards(vec3 *p0, vec3 *p1, vec3 *v);

float vec3Sign(vec3 *p0, vec3 *p1, vec3 *v);

#endif
