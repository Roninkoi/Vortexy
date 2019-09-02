#include "field.h"

void fieldInit(Field *f, unsigned int r)
{
	// set resolution
	f->res = r;

	// allocate memory for 2d array
	f->f = (vec4 **) malloc(f->res * sizeof(vec4 *));

	for (unsigned int i = 0; i < f->res; ++i) {
		f->f[i] = (vec4 *) malloc(f->res * sizeof(vec4));
	}

	// initialize to zero
	fieldSet(f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void fieldDestroy(Field *f)
{
	for (unsigned int i = 0; i < f->res; ++i) {
		free(f->f[i]);
	}

	free(f->f);
}

void fieldSet(Field *f, float x, float y, float z, float w)
{
	for (unsigned int i = 0; i < f->res; ++i) {
		for (unsigned int j = 0; j < f->res; ++j) {
			f->f[i][j].x = x;
			f->f[i][j].y = y;
			f->f[i][j].z = z;
			f->f[i][j].w = w;
		}
	}
}

void fieldPrint(Field *f)
{

	for (unsigned int i = 0; i < f->res; ++i) {
		for (unsigned int j = 0; j < f->res; ++j) {
			vec4Print(&f->f[i][j]);
		}
	}
}

// loads an array of bytes into a field
void fieldLoader(Field *f, int width, int height, unsigned char *data)
{
	unsigned char c;

	int ti = 0;
	int ch = 0;

	fieldInit(f, width > height ? width : height);

	for (int i = 0; data[i]; ++i) {
		c = data[i];
		float val = (float) c / 255.0f;

		switch (ch % 3) {
			case 0:
				f->f[ti % f->res][(int) (ti / f->res)].x = val;
				break;
			case 1:
				f->f[ti % f->res][(int) (ti / f->res)].y = val;
				break;
			case 2:
				f->f[ti % f->res][(int) (ti / f->res)].z = val;
				break;
		}

		++ch; // channel

		if (ch % 3 == 0) {
			++ti; // index
		}
	}

	free(data);
}

unsigned char *fieldBytes(Field *f)
{
	unsigned char *data = NULL;

	return data;
}