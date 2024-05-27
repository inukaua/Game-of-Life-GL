#version 330 core
out vec4 FragColor;

uniform sampler2D GOL_canvas;

in VS_OUTPUT
{
    vec2 texCoord;
} IN;

void main()
{
    FragColor = texture(GOL_canvas, IN.texCoord).xxxx;
} 