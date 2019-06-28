#version 330 core
#extension GL_ARB_separate_shader_objects : enable

in vec2 inPosition;
in vec2 inTexcoord;

out vec2 fragTexcoord;

void main(void)
{
    fragTexcoord = inTexcoord;

    vec2 position = vec2(inPosition.x, -inPosition.y);

    gl_Position = vec4(position, -0.5, 1.0);
}