#ifndef TEX_LOADER_H
#define TEX_LOADER_H

#include "phys/field.h"

// loads a ppm image into a field
void ppmLoader(struct Field *f, char *path)
{
	FILE *fp;
	char *data;
	size_t fsize;

	fp = fopen(path, "r");

	if (!fp) {
		exit(1);
	}

	data = (char *) malloc(0x100000);
	fsize = fread(data, 1, 0x100000, fp);

	free(data);
}

#endif
