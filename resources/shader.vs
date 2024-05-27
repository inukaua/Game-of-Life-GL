#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out VS_OUTPUT
{
    vec2 texCoord;
} OUT;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    OUT.texCoord = (aTex+1.0)/2.0;
}