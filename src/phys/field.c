#include <phys/field.h>

void p_fieldInit(struct Field* f, unsigned int r)
{
  // set resolution
  f->res = r;
  
  // allocate memory for 2d array
  f->f = (p_vec4**) malloc(f->res * sizeof(p_vec4*));

  for (int i = 0; i < f->res; ++i) {
    f->f[i] = (p_vec4*) malloc(f->res * sizeof(p_vec4));
  }

  // initialize to zero
  p_fieldSet(f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void p_fieldSet(struct Field* f, float x, float y, float z, float w)
{
  for (int i = 0; i < f->res; ++i) {
    for (int j = 0; j < f->res; ++j) {
      f->f[i][j].x = x;
      f->f[i][j].y = y;
      f->f[i][j].z = z;
      f->f[i][j].w = w;
    }
  }
}

