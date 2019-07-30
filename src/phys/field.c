#include "phys/field.h"

void p_fieldInit(Field *f, unsigned int r)
{
	// set resolution
	f->res = r;

	// allocate memory for 2d array
	f->f = (vec4 **) malloc(f->res * sizeof(vec4 *));

	for (int i = 0; i < f->res; ++i) {
		f->f[i] = (vec4 *) malloc(f->res * sizeof(vec4));
	}

	// initialize to zero
	p_fieldSet(f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void p_fieldDestroy(Field *f) // check this later
{
	for (int i = 0; i < f->res; ++i) {
		free(f->f[i]);
	}

	free(f->f);
}

void p_fieldSet(Field *f, float x, float y, float z, float w)
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

void p_fieldPrint(Field *f)
{

	for (int i = 0; i < f->res; ++i) {
		for (int j = 0; j < f->res; ++j) {
			p_vec4Print(&f->f[i][j]);
		}
	}
}

// loads an array of bytes into a field
void p_fieldLoader(Field *f, int width, int height, unsigned char *data)
{
	unsigned char c;

	int ti = 0;
	int ch = 0;

	p_fieldInit(f, width > height ? width : height);

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
			++ti; // texture index
		}
	}

	free(data);
}

unsigned char *p_fieldBytes(Field *f)
{
	unsigned char *data;
	return data;
}