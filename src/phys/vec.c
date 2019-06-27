#include <phys/vec.h>

p_vec4 p_vec4Make(float x, float y, float z, float w)
{
  p_vec4 v;

  p_vec4Init(&v, x, y, z, w);

  return v;
}

void p_vec4Init(p_vec4* v, float x, float y, float z, float w)
{
  v->x = x;
  v->y = y;
  v->z = z;
  v->w = w;
}

void p_vec4Print(p_vec4* v)
{
  printf("(%.1f, %.1f, %.1f, %.1f)\n", v->x, v->y, v->z, v->w);
}
