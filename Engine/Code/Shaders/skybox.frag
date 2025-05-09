#version 450

layout(set = 1, binding = 1) uniform samplerCube per_cubemap;

layout(location = 0) in vec3 inPos;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = texture(per_cubemap, inPos);
}