#version 130

out vec4 fCol;

in vec4 vPos;
in vec4 vTex;
in vec4 vNorm;
in vec4 vCol;

uniform sampler2D tex;
uniform float rs;

void main()
{
    vec4 col = texture(tex, vTex.st) * vCol;

    col /= max(exp(vPos.z*rs), 1.0f);

    fCol = col;

    if (fCol.a == 0.0f)
        discard;
}
