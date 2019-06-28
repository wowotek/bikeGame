#version 330 core
#extension GL_ARB_seperate_shader_objects : enable

in vec2 fragTexcoord;

uniform sampler2D iColorTexture;

out vec4 outColor;

void main(void)
{
    outColor = texture(iColorTexture, fragTexcoord);
}