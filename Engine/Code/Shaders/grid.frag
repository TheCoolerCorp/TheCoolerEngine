#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 inFragUV;
layout(location = 1) in mat4 inInvVP;
layout(location = 5) in vec3 incamPos;

float grid(vec2 coord, float gridSize, float lineThickness) {
    vec2 gridMod = abs(fract(coord / gridSize - 0.5) - 0.5) / fwidth(coord);
    float line = min(gridMod.x, gridMod.y);
    return 1.0 - smoothstep(0.0, lineThickness, line);
}

void main() 
{
    vec2 screenUV = inFragUV * 2.0 - 1.0;
    vec4 worldPosNear = inInvVP * vec4(screenUV, 0.0, 1.0);
    vec4 worldPosFar = inInvVP * vec4(screenUV, 1.0, 1.0);
    worldPosNear /= worldPosNear.w;
    worldPosFar /= worldPosFar.w;

    vec3 rayDir = normalize(worldPosFar.xyz - worldPosNear.xyz);
    if (abs(rayDir.y) < 1e-5) discard;

    float t = -worldPosNear.y / rayDir.y;
    if (t < 0.0) discard;

    vec3 pos = worldPosNear.xyz + rayDir * t;
    pos.y -= 0.001;

    float majorGrid = grid(pos.xz, 3.0, 0.4);
    float minorGrid = grid(pos.xz, 1.5, 0.2);

    float d = distance(incamPos, pos);
    float fade = clamp(1.0 - d / 100.0, 0.0, 1.0);

    vec3 baseColor = mix(vec3(0.05), vec3(0.3), majorGrid) + vec3(0.1) * minorGrid;
    float alpha = max(majorGrid, minorGrid) * fade;

    if (alpha < 0.01) discard;

    outColor = vec4(baseColor, alpha);
}