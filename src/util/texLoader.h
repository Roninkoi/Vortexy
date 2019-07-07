#ifndef TEX_LOADER_H
#define TEX_LOADER_H

#include "phys/field.h"

// loads a ppm image into a field
void ppmLoader(struct Field *f, char *path)
{
  FILE *fp;
  char *src;
  size_t src_size;

  fp = fopen("data/test.ppm", "r");

  if (!fp) {
    exit(1);
  }

  src = (char *) malloc(0x100000);
  src_size = fread(src, 1, 0x100000, fp);

  printf("%i\n", src_size);
}

#endif
