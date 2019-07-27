#version 150

in vec4 pos;
in vec4 tex;
in vec4 norm;
in vec4 col;

out vec4 vPos;
out vec4 vTex;
out vec4 vNorm;
out vec4 vCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = model * pos;
    gl_Position = vec4(gl_Position.xyz, 1.0f);

    vPos = pos;
    vTex = vec4(tex.s / tex.p, tex.t / tex.q, 1.0f, 1.0f);
    vNorm = norm;
    vCol = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}
