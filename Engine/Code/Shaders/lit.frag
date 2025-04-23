#version 450

// Textures
layout(set = 1, binding = 1) uniform sampler2D per_albedoMap;
layout(set = 1, binding = 2) uniform sampler2D per_normalMap;
layout(set = 1, binding = 3) uniform sampler2D per_metallicMap;
layout(set = 1, binding = 4) uniform sampler2D per_RoughnessMap;
layout(set = 1, binding = 5) uniform sampler2D per_aoMap;

// Values
layout(set = 1, binding = 6) uniform TexturesValues
{
    vec3 AsAlbedoTexture;
    float AsMetallicTexture;
    float AsRoughnessTexture;
    float AsAotexture;
} per_texsVal;

// Check if a texture is passed to the shader or values are used
layout(set = 1, binding = 7) uniform HasTextures 
{
    bool AsAlbedoTexture;
    bool AsNormalTexture;
    bool AsMetallicTexture;
    bool AsRoughnessTexture;
    bool AsAotexture;
} per_bTexs;


// Lights
layout(set = 0, binding = 1) uniform LightData 
{
    vec3 lightPosition;
    vec3 lightColor;
    float intensity;
} common_light;



layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;


void main() 
{
    outColor = texture(per_albedoMap, fragTexCoord);

}