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

    // move the cube to follow the camera (simulate view matrix without translation)
    vec3 worldPos = inPos + common_camModel.pos;

    // transform with original VP
    vec4 clipPos = common_camModel.vp * vec4(worldPos, 1.0);

    // force to far plane: set z = w, little 0.9999 to push in the far clip, avoids depth precision limits
    gl_Position = vec4(clipPos.xy, clipPos.w * 0.9999, clipPos.w);

}
