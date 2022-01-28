#version 430 core
layout (location = 0) in vec2 position;

uniform mat4 u_Projection;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_Model * vec4(position, 0.0, 1.0);
}
