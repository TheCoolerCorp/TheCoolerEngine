#version 450

layout(set = 0, binding = 0) uniform CameraUBO
{
    mat4 vp;
    vec3 pos;
} common_camModel;

layout(location = 0) out vec2 outFragUV;
layout(location = 1) out mat4 outInvVP;
layout(location = 5) out vec3 outcamPos;

vec2 positions[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
);

void main() 
{
    outFragUV = positions[gl_VertexIndex] * 0.5 + 0.5;
    outInvVP = inverse(common_camModel.vp);
    outcamPos = common_camModel.pos;

    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}