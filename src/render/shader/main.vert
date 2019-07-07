#version 330 core

layout (location = 0) in vec3 pos;

out vec4 vCol;

void main()
{
    gl_Position = vec4(pos, 1.0f);

    vCol = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}
