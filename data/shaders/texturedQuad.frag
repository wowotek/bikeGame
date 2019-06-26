#version 330 core
#extension GL_ARB_seperate_shader_objects : enable

in vec2 fragTexcoord;

out vec4 outColor;

void main(void)
{
    outColor = vec4(fragTexcoord, 0.0, 1.0);
}