#include "sim.h"
#include "render/shader.h"

void r_loadShader(struct Shader *s, char *vertPath, char *fragPath)
{
	s->vertShader = glCreateShader(GL_VERTEX_SHADER);
	s->fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// load vertex shader
	FILE *vert_fp;
	const char *vert_data;
	size_t vert_fsize;

	vert_fp = fopen(vertPath, "r");

	if (!vert_fp) {
		exit(1);
	}

	vert_data = (char *) malloc(SRC_MAX);
	vert_fsize = fread(vert_data, 1, SRC_MAX, vert_fp);

	// load fragment shader
	FILE *frag_fp;
	const char *frag_data;
	size_t frag_fsize;

	frag_fp = fopen(fragPath, "r");

	if (!frag_fp) {
		exit(1);
	}

	frag_data = (char *) malloc(SRC_MAX);
	frag_fsize = fread(frag_data, 1, SRC_MAX, frag_fp);

	GLint r = GL_FALSE;
	int ll;

	// compile vert
	printf("Compiling %s\n", vertPath);

	glShaderSource(s->vertShader, 1, &vert_data, NULL);
	glCompileShader(s->vertShader);

	glGetShaderiv(s->vertShader, GL_COMPILE_STATUS, &r);
	glGetShaderiv(s->vertShader, GL_INFO_LOG_LENGTH, &ll);

	if (ll > 0) {
		char *err = malloc(sizeof(char) * (ll + 1));

		glGetShaderInfoLog(s->vertShader, ll, NULL, err);
		printf("%s\n", err);

		free(err);
	}

	// compile frag
	printf("Compiling %s\n", fragPath);

	glShaderSource(s->fragShader, 1, &frag_data, NULL);
	glCompileShader(s->fragShader);

	glGetShaderiv(s->fragShader, GL_COMPILE_STATUS, &r);
	glGetShaderiv(s->fragShader, GL_INFO_LOG_LENGTH, &ll);

	if (ll > 0) {
		char *err = malloc(sizeof(char) * (ll + 1));

		glGetShaderInfoLog(s->fragShader, ll, NULL, err);
		printf("%s\n", err);

		free(err);
	}

	// link program
	printf("Linking shader...\n");

	s->program = glCreateProgram();

	glAttachShader(s->program, s->vertShader);
	glAttachShader(s->program, s->fragShader);

	glLinkProgram(s->program);

	glGetProgramiv(s->program, GL_LINK_STATUS, &r);
	glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &ll);

	if (ll > 0) {
		char *err = malloc(sizeof(char) * (ll + 1));

		glGetProgramInfoLog(s->program, ll, NULL, err);
		printf("%s\n", err);

		free(err);
	}

	glDetachShader(s->program, s->vertShader);
	glDetachShader(s->program, s->fragShader);

	glDeleteShader(s->vertShader);
	glDeleteShader(s->fragShader);
}

void r_destroyShader(struct Shader *s)
{
	glDeleteProgram(s->program);
}
