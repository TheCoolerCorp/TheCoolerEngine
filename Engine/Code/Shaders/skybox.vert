#version 450

layout(set = 0, binding = 0) uniform CameraUBO
{
    mat4 vp;
    vec3 pos;
} common_camModel;

layout(location = 0) in vec3 inTexCoords;

layout(location = 0) out vec3 outTexCoord;

void main()
{
    outTexCoord = inTexCoords;
    gl_Position = (common_camModel.vp * vec4(inTexCoords, 1.0)).xyzz;
} 
