#version 330 core
#extension GL_ARB_separate_shader_objects : enable

in vec2 inPosition;
in vec2 inTexcoord;

out vec2 fragTexcoord;

void main(void)
{
    fragTexcoord = inTexcoord;
    gl_Position = vec4(inPosition, -0.5, 1.0);
}