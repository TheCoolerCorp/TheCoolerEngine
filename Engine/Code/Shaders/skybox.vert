#version 450

layout(set = 0, binding = 0) uniform CameraUBO
{
    mat4 vp;
    vec3 pos;
} common_camModel;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outPos;


void main()
{
    outPos = inPos;
    gl_Position = (common_camModel.vp * vec4(inPos, 1.0));
} 
