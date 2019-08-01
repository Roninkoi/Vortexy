#include "phys/vec.h"

vec4 p_vec4(float x, float y, float z, float w)
{
	vec4 v;

	p_vec4Init(&v, x, y, z, w);

	return v;
}

void p_vec4Init(vec4 *v, float x, float y, float z, float w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void p_vec4Print(vec4 *v)
{
	printf("(%.1f, %.1f, %.1f, %.1f)\n", v->x, v->y, v->z, v->w);
}

vec4 p_ivec4()
{
	return p_vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

vec4 p_nvec4()
{
	return p_vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void p_vec4Add(vec4 *v0, vec4 *v1) // v0 += v1
{
	v0->x += v1->x;
	v0->y += v1->y;
	v0->z += v1->z;
	v0->w += v1->w;
}

void p_vec4Sub(vec4 *v0, vec4 *v1) // v0 -= v1
{
	v0->x -= v1->x;
	v0->y -= v1->y;
	v0->z -= v1->z;
	v0->w -= v1->w;
}

void p_vec4Mul(vec4 *v, float s)
{
	v->x *= s;
	v->y *= s;
	v->z *= s;
	v->w *= s;
}

void p_vec4Div(vec4 *v, float s)
{
	v->x /= s;
	v->y /= s;
	v->z /= s;
	v->w /= s;
}

float p_vec3Len(vec4 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

float p_vec4Len(vec4 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

// dot product
float p_vec4Dot(vec4 *v0, vec4 *v1)
{
	return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z + v0->w * v1->w;
}

// cross product
vec4 p_vec4Cross(vec4 *v0, vec4 *v1)
{
	vec4 v;

	v.x = v0->y * v1->z - v0->z * v1->y;
	v.y = v0->z * v1->x - v0->x * v1->z;
	v.z = v0->x * v1->y - v0->y * v1->x;

	return v;
}

void p_vec4Normalize(vec4 *v)
{
	p_vec4Div(v, p_vec4Len(v));
}
