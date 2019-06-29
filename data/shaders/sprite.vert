#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 iPosition;
layout(location = 1) in vec2 iTexcoord;

out vec2 fragTexcoord;

uniform vec2 iModelPosition;
uniform vec2 iModelAngle;

uniform vec2 iCameraPosition;
uniform float iCameraAspect = 1.0;
uniform float iCameraZoom = 0.0;
uniform float iCameraAngle = 0.0;

void main (void)
{
    fragTexcoord = iTexcoord;
    
    float expZoom = exp(iCameraZoom);

    mat3 proj = mat3(iCameraAspect * expZoom, 0, 0, 0, expZoom, 0, 0, 0, 1);
    mat3 view = mat3(1, 0, 0, 0, 1, 0, -iCameraPosition.x, -iCameraPosition.y, 1);
    mat3 model = mat3(1, 0, 0, 0, 1, 0, iModelPosition.x, iModelPosition.y, 1);

    mat3 mvp = proj * view * model;

    vec2 position = vec2(mvp * vec3(iPosition*vec2(1,-1), 1));

    gl_Position = vec4(position, 0.0, 1.0);
}