#version 450

layout(set = 0, binding = 0) uniform CameraUBO
{
    mat4 vp;
} common_camubo;

layout(set = 1, binding = 0) uniform ObjectUBO
{
    mat4 model;
} per_objubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 fragTexCoord;


void main() {
    gl_Position = common_camubo.vp * per_objubo.model * vec4(inPosition,  1.0);
    fragTexCoord = inTexCoord;
    normal = inNormal;
}