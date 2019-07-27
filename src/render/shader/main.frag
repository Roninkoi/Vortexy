#version 150

out vec4 fCol;

in vec4 vPos;
in vec4 vTex;
in vec4 vNorm;
in vec4 vCol;

uniform sampler2D tex;

void main()
{
    fCol = texture(tex, vTex.st) * vCol;
} 
