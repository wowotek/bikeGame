#version 330 core
#extension GL_ARB_seperate_shader_objects : enable

uniform vec4 iColor;

out vec4 oColor;

void main (void)
{
    oColor = iColor;
}