#ifndef TEX_LOADER_H
#define TEX_LOADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEX_MAX 0x100000

// loads a ppm image into an array of bytes (RGB)
unsigned char *ppmLoader(char *path, int *texWidth, int *texHeight)
{
	char *data = NULL;
	FILE *fp;
	size_t fsize;

	fp = fopen(path, "r");

	if (!fp) {
		printf("Not found: %s\n", path);
		exit(1);
	}

	data = calloc(TEX_MAX, sizeof(char *));
	fsize = fread(data, 1, TEX_MAX, fp);

	fclose(fp);

	char *c = malloc(TEX_MAX);

	int width = 0, height = 0;
	int dp = 0;
	int ds = 0;

	float maxval = 0.0f;

	unsigned char *returns = NULL;
	int ri = 0;

	for (int i = 0; data[i] && i < TEX_MAX; ++i) {
		int j = 0;

		ds = (dp ? data[i] != ' ' : 1);

		for (j = 0; data[i] != '\n' && data[i] != '\r' && ds && data[i]; ++j) {
			c[j] = data[i];
			++i;
		}

		c[j] = 0;

		if (c[0] != '#' && c[0] != 'P') {
			if (width == 0 && height == 0) { // parse dimensions
				char ws[6];
				char hs[6];

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

				returns = malloc(sizeof(unsigned char) * width * height * 3);

				dp = 1;
			}
			else { // parse texture data
				if (maxval == 0.0f) {
					maxval = (float) atoi(c);

					continue; // proper data starts after
				}

				float val = (float) atoi(c) / maxval;

				if (ri < width * height * 3)
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

unsigned char *ppmLoaderAlpha(char *path, int *texWidth, int *texHeight, int r, int g, int b)
{
	unsigned char *rgb = ppmLoader(path, texWidth, texHeight);

	unsigned char *rgba = malloc(sizeof(unsigned char) * (*texWidth) * (*texHeight) * 4);

	for (int i = 0; i < (*texWidth) * (*texHeight); i += 1) {
		rgba[i * 4 + 0] = rgb[i * 3 + 0];
		rgba[i * 4 + 1] = rgb[i * 3 + 1];
		rgba[i * 4 + 2] = rgb[i * 3 + 2];
		rgba[i * 4 + 3] = 255;

		if (rgb[i * 3 + 0] == r && rgb[i * 3 + 1] == g && rgb[i * 3 + 2] == b) { // bitmasking
			rgba[i * 4 + 3] = 0;
		}
	}

	free(rgb);

	return rgba;
}

#endif
