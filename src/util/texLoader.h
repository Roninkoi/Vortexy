#ifndef TEX_LOADER_H
#define TEX_LOADER_H

#include <string.h>

#include "phys/field.h"

#define TEXMAX 0x100000

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

	data = (char *) malloc(TEXMAX);
	fsize = fread(data, 1, TEXMAX, fp);

	char *c = (char *) malloc(TEXMAX);

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
				char *ws = (char *) malloc(sizeof(char) * 3);
				char *hs = (char *) malloc(sizeof(char) * 3);

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

				++ti; // texture index
				++ch; // channel
			}
		}
	}

	free(data);
	free(c);
}

#endif
