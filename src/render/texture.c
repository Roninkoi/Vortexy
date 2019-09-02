#include "texture.h"
#include "util/texLoader.h"

void r_flatTex(Texture *t,
			   unsigned char r, unsigned char g, unsigned char b,
			   int w, int h)
{
	glGenTextures(1, &t->tex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->tex);

	t->w = w;
	t->h = h;

	unsigned char *image = malloc(sizeof(unsigned char) * w * h * 3);

	for (int i = 0; i < w * h; i += 1) {
		image[i * 3 + 0] = r;
		image[i * 3 + 1] = g;
		image[i * 3 + 2] = b;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, t->w, t->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//free(image);
	t->data = image;
}

void r_loadTex(Texture *t, char *path)
{
	t->path = path;

	glGenTextures(1, &t->tex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->tex);

	unsigned char *image = ppmLoader(path, &t->w, &t->h);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, t->w, t->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//free(image);
	t->data = image;
}

/*void r_fieldToTex(Field *f, Texture *t)
{
	glGenTextures(1, &t->tex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->tex);

	t->w = f->res;
	t->h = f->res;

	unsigned char *image = p_fieldBytes(f);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, t->w, t->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//free(image);
	t->data = image;
}*/

void r_destroyTex(Texture *t)
{
	if (t->tex > 0) {
		glDeleteTextures(1, &t->tex);
		t->tex = 0;
	}
	free(t->data);
}
