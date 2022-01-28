#version 430 core

layout(location = 0) out vec3 colour;

uniform vec3 u_Colour;

void main()
{
    colour = u_Colour;
}
