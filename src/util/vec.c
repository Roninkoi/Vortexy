#include "vec.h"
#include "util.h"

/*
  VEC4
 */

vec4 Vec4(real x, real y, real z, real w)
{
	vec4 v;

	vec4Init(&v, x, y, z, w);

	return v;
}

void vec4Init(vec4 *v, real x, real y, real z, real w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void vec4Print(vec4 *v)
{
	printf("(%.1f, %.1f, %.1f, %.1f)\n", v->x, v->y, v->z, v->w);
}

vec4 ivec4()
{
	return Vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

vec4 nvec4()
{
	return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void vec4Add(vec4 *v0, vec4 *v1) // v0 += v1
{
	v0->x += v1->x;
	v0->y += v1->y;
	v0->z += v1->z;
	v0->w += v1->w;
}

void vec4Sub(vec4 *v0, vec4 *v1) // v0 -= v1
{
	v0->x -= v1->x;
	v0->y -= v1->y;
	v0->z -= v1->z;
	v0->w -= v1->w;
}

void vec4Mul(vec4 *v, real s)
{
	v->x *= s;
	v->y *= s;
	v->z *= s;
	v->w *= s;
}

void vec4Div(vec4 *v, real s)
{
	v->x /= s;
	v->y /= s;
	v->z /= s;
	v->w /= s;
}

real vec4Len3(vec4 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

real vec4Len(vec4 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

// dot product
real vec4Dot(vec4 *v0, vec4 *v1)
{
	return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z + v0->w * v1->w;
}

// cross product
vec4 vec4Cross(vec4 *v0, vec4 *v1)
{
	vec4 v;

	v.x = v0->y * v1->z - v0->z * v1->y;
	v.y = v0->z * v1->x - v0->x * v1->z;
	v.z = v0->x * v1->y - v0->y * v1->x;
	v.w = 0.0f;

	return v;
}

void vec4Normalize(vec4 *v)
{
	vec4Div(v, vec4Len(v));
}

int vec4Equ(vec4 *v0, vec4 *v1)
{
	if (v0->x == v1->x && v0->y == v1->y && v0->z == v1->z && v0->w == v1->w)
		return 1;
	
	return 0;
}

int vec4Equ3(vec4 *v0, vec4 *v1)
{
	if (v0->x == v1->x && v0->y == v1->y && v0->z == v1->z)
		return 1;
	
	return 0;
}

vec4 vec4Copy(vec4 *v)
{
	return Vec4(v->x, v->y, v->z, v->w);
}

vec4 vec4Copy3(vec3 *v)
{
	return Vec4(v->x, v->y, v->z, 0.0f);
}

/*
  VEC3
 */

vec3 Vec3(real x, real y, real z)
{
	vec3 v;

	vec3Init(&v, x, y, z);

	return v;
}

void vec3Init(vec3 *v, real x, real y, real z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

void vec3Print(vec3 *v)
{
	printf("(%.10f, %.10f, %.10f)\n", v->x, v->y, v->z);
}

vec3 ivec3()
{
	return Vec3(1.0f, 1.0f, 1.0f);
}

vec3 nvec3()
{
	return Vec3(0.0f, 0.0f, 0.0f);
}

void vec3Add(vec3 *v0, vec3 *v1) // v0 += v1
{
	v0->x += v1->x;
	v0->y += v1->y;
	v0->z += v1->z;
}

void vec3Sub(vec3 *v0, vec3 *v1) // v0 -= v1
{
	v0->x -= v1->x;
	v0->y -= v1->y;
	v0->z -= v1->z;
}

void vec3Mul(vec3 *v, real s)
{
	v->x *= s;
	v->y *= s;
	v->z *= s;
}

void vec3Div(vec3 *v, real s)
{
	v->x /= s;
	v->y /= s;
	v->z /= s;
}

real vec3Len(vec3 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

// dot product
real vec3Dot(vec3 *v0, vec3 *v1)
{
	return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

// cross product
vec3 vec3Cross(vec3 *v0, vec3 *v1)
{
	vec3 v;

	v.x = v0->y * v1->z - v0->z * v1->y;
	v.y = v0->z * v1->x - v0->x * v1->z;
	v.z = v0->x * v1->y - v0->y * v1->x;

	return v;
}

void vec3Normalize(vec3 *v)
{
	vec3Div(v, vec3Len(v));
}

int vec3Equ(vec3 *v0, vec3 *v1)
{
	if (v0->x == v1->x && v0->y == v1->y && v0->z == v1->z)
		return 1;
	
	return 0;
}

vec3 vec3Copy(vec3 *v)
{
	return Vec3(v->x, v->y, v->z);
}

vec3 vec3Outwards(vec3 *p0, vec3 *p1, vec3 *v)
{
	vec3 r;
	vec3 d = vec3Copy(p1);
	vec3Sub(&d, p0);

	r = vec3Copy(v);

	real s = sign(vec3Dot(v, &d));

	vec3Mul(&r, s);

	return r;
}

real vec3Sign(vec3 *p0, vec3 *p1, vec3 *v)
{
	vec3 r;
	vec3 d = vec3Copy(p1);
	vec3Sub(&d, p0);

	return sign(vec3Dot(v, &d));
}
