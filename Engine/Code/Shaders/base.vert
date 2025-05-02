#version 450

layout(set = 0, binding = 0) uniform CameraUBO
{
    mat4 vp;
    vec3 pos;
} common_camModel;

layout(set = 1, binding = 0) uniform ObjectUBO
{
    mat4 model;
    mat4 normalMatrix;
} per_objModel;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out vec3 outWorldPos;
layout(location = 3) out vec3 outCamPos;


void main() 
{
    outWorldPos = vec3(per_objModel.model * vec4(inPosition, 1.0));

    vec3 t_normal = mat3(per_objModel.normalMatrix) * inNormal;
    outNormal = normalize(t_normal);

    outTexCoord = inTexCoord;

    outCamPos = common_camModel.pos;

    gl_Position = common_camModel.vp * vec4(outWorldPos, 1.0);

}