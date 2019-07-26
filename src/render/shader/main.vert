#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 tex;
layout (location = 2) in vec4 norm;
layout (location = 3) in vec4 col;

out vec4 vPos;
out vec4 vTex;
out vec4 vNorm;
out vec4 vCol;

void main()
{
    vPos = pos;
    gl_Position = vPos;

    vTex = vec4(tex.s / tex.p, tex.t / tex.q, 1.0f, 1.0f);
    vNorm = norm;
    vCol = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}
