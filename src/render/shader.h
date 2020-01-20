#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#define SHADER_MAX 0x100000

struct Shader {
	GLuint program;

	GLuint vertShader;
	GLuint fragShader;
};

// load and compile GLSL shader for rendering
void r_loadShader(struct Shader *s, char *vertPath, char *fragPath);

void r_destroyShader(struct Shader *s);

#endif
