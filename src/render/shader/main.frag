#version 150

#define TRICOL 0

out vec4 fCol;

in vec4 vPos;
in vec4 vTex;
in vec4 vNorm;
in vec4 vCol;

uniform sampler2D tex;

void main()
{
    vec4 col = texture(tex, vTex.st) * vCol;

    col /= max(exp(vPos.z*0.2f), 1.0f);

	fCol = col;

    if (TRICOL == 1)
        fCol *= float(gl_PrimitiveID % 4 + 1) * 0.25f * 2.0f;

    if (fCol.a == 0.0f)
        discard;
}
