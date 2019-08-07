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

    fCol /= max(exp(vPos.z*0.5f), 1.0f);

    fCol *= float(gl_PrimitiveID % 4 + 1) * 0.25f * 2.0f;

    if (fCol.a == 0.0f)
        discard;
} 
