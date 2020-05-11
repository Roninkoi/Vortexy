#version 130

out vec4 fCol;

in vec4 vPos;
in vec4 vTex;
in vec4 vNorm;
in vec4 vCol;

uniform sampler2D tex;
uniform float rs;

#define ZFAR 10.0f

void main()
{
    vec4 col = texture(tex, vTex.st) * vCol;

    col.a *= exp(-abs(vPos.z / rs / ZFAR));

    fCol = col;
}
