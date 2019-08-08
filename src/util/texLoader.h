#ifndef TEX_LOADER_H
#define TEX_LOADER_H

#include <string.h>

#include "phys/field.h"

#define TEX_MAX 0x100000

// loads a ppm image into an array of bytes (RGB)
unsigned char *ppmLoader(char *path, int *texWidth, int *texHeight)
{
	char *data = NULL;
	FILE *fp;
	size_t fsize;

	fp = fopen(path, "r");

	if (!fp) {
		exit(1);
	}

	data = (char *) malloc(TEX_MAX);
	fsize = fread(data, 1, TEX_MAX, fp);

	fclose(fp);

	char *c = (char *) malloc(TEX_MAX);

	int width = 0, height = 0;
	int dp = 0;
	int ds = 0;

	float maxval = 0.0f;

	unsigned char *returns = NULL;
	int ri = 0;

	for (int i = 0; data[i]; ++i) {
		int j = 0;

		ds = (dp ? data[i] != ' ' : 1);

		for (j = 0; data[i] != '\n' && data[i] != '\r' && ds && data[i]; ++j) {
			c[j] = data[i];
			++i;
		}

		c[j] = 0;

		if (c[0] != '#' && c[0] != 'P') {
			if (width == 0 && height == 0) { // parse dimensions
				char *ws = (char *) malloc(sizeof(char) * 6);
				char *hs = (char *) malloc(sizeof(char) * 6);

				int k = 0;
				for (j = 0; c[j] != ' ' && c[j]; ++j) { // texture width
					ws[k] = c[j];
					++k;
				}

				ws[k] = 0;

				k = 0;
				for (j = 0; c[j] != ' ' && c[j]; ++j) { // texture height
					hs[k] = c[j];
					++k;
				}

				hs[k] = 0;

				width = atoi(ws);
				height = atoi(hs);

				returns = (unsigned char *) malloc(sizeof(unsigned char) * width * height * 3);

				free(ws);
				free(hs);

				dp = 1;
			}
			else { // parse texture data
				if (maxval == 0.0f) {
					maxval = (float) atoi(c);

					continue; // proper data starts after
				}

				float val = (float) atoi(c) / maxval;

				returns[ri] = (unsigned char) (val * 255.0f);

				++ri;
			}
		}
	}

	free(data); // corrupted unsorted chunks
	free(c);

	*texWidth = width;
	*texHeight = height;

	return returns;
}

unsigned char *ppmLoaderAlpha(char *path, int *texWidth, int *texHeight)
{
	unsigned char *rgb = ppmLoader(path, texWidth, texHeight);

	unsigned char *rgba = (unsigned char *) malloc(sizeof(unsigned char) * (*texWidth) * (*texHeight) * 4);

	for (int i = 0; i < (*texWidth) * (*texHeight); i += 1) {
		rgba[i * 4 + 0] = rgb[i * 3 + 0];
		rgba[i * 4 + 1] = rgb[i * 3 + 1];
		rgba[i * 4 + 2] = rgb[i * 3 + 2];
		rgba[i * 4 + 3] = 255;

		if (rgb[i * 3 + 0] == 255 && rgb[i * 3 + 1] == 0 && rgb[i * 3 + 2] == 255) { // bitmasking pink
			rgba[i * 4 + 3] = 0;
		}
	}

	free(rgb);

	return rgba;
}

#endif
