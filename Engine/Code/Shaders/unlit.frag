#version 450

layout(set = 1, binding = 1) uniform sampler2D per_albedoMap;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inWorldPos;
layout(location = 3) in vec3 inCamPos;

layout(location = 0) out vec4 outColor;




void main() 
{
    outColor = texture(per_albedoMap, inTexCoord);

}