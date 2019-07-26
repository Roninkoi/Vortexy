#ifndef TEX_LOADER_H
#define TEX_LOADER_H

#include <string.h>

#include "phys/field.h"

#define TEX_MAX 0x100000

// loads a ppm image into a field
void ppmFieldLoader(Field *f, char *path)
{
	FILE *fp;
	char *data;
	size_t fsize;

	fp = fopen(path, "r");

	if (!fp) {
		exit(1);
	}

	data = (char *) malloc(TEX_MAX);
	fsize = fread(data, 1, TEX_MAX, fp);

	char *c = (char *) malloc(TEX_MAX);

	int width = 0, height = 0;
	int dp = 0;
	int ds = 0;

	float maxval = 0.0f;

	int ti = 0;
	int ch = 0;

	for(int i = 0; data[i]; ++i) {
		int j;

		ds = (dp ? data[i] != ' ' : 1);

		for(j = 0; data[i] != '\n' && ds && data[i]; ++j) {
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

				f->res = width > height ? width : height;

				p_fieldInit(f, f->res); // allocate res x res field

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

				switch(ch % 3) {
					case 0:
						f->f[ti % f->res][(int)(ti / f->res)].x = val;
						break;
					case 1:
						f->f[ti % f->res][(int)(ti / f->res)].y = val;
						break;
					case 2:
						f->f[ti % f->res][(int)(ti / f->res)].z = val;
						break;
				}

				++ch; // channel

				if (ch % 3 == 0) {
					++ti; // texture index
				}
			}
		}
	}

	free(data);
	free(c);
}

// loads a ppm image into an array of bytes (RGB)
unsigned char *ppmLoader(char *path, int *texWidth, int *texHeight)
{
	FILE *fp;
	char *data;
	size_t fsize;

	fp = fopen(path, "r");

	if (!fp) {
		exit(1);
	}

	data = (char *) malloc(TEX_MAX);
	fsize = fread(data, 1, TEX_MAX, fp);

	char *c = (char *) malloc(TEX_MAX);

	int width = 0, height = 0;
	int dp = 0;
	int ds = 0;

	float maxval = 0.0f;

	unsigned char *returns;
	int ri = 0;

	for(int i = 0; data[i]; ++i) {
		int j;

		ds = (dp ? data[i] != ' ' : 1);

		for(j = 0; data[i] != '\n' && ds && data[i]; ++j) {
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

	free(data);
	free(c);

	*texWidth = width;
	*texHeight = height;

	return returns;
}

#endif