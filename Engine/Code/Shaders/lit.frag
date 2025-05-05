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
    vec3 albedoTexture;
    float metallicTexture;
    float roughnessTexture;
    float aoTexture;
} per_texsVal;

// Check if a texture is passed to the shader or values are used
layout(set = 1, binding = 7) uniform HasTextures 
{
    bool HasAlbedoTexture;
    bool HasNormalTexture;
    bool HasMetallicTexture;
    bool HasRoughnessTexture;
    bool HasAoTexture;
} per_bTexs;


// Lights
layout(set = 2, binding = 0) uniform LightData
{
    vec3 position;
    vec3 color;
    float intensity;
} light_lhtValues;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inWorldPos;
layout(location = 3) in vec3 inCamPos;


layout(location = 0) out vec4 outColor;


const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(per_normalMap, inTexCoord).xyz;
    tangentNormal = tangentNormal * 2.0 - 1.0;

    vec3 Q1  = dFdx(inWorldPos);
    vec3 Q2  = dFdy(inWorldPos);
    vec2 st1 = dFdx(inTexCoord);
    vec2 st2 = dFdy(inTexCoord);

    vec3 N   = normalize(inNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    // MAYBE PUT A - ON B ?? Depend on righ handed or left handed
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);

    //return T;
    //return tangentNormal;
    //return vec3(0.0, 0.0, T.z);
}

vec3 Radiance(vec3 lightDir)
{
    float distance = length(lightDir);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light_lhtValues.color * attenuation * light_lhtValues.intensity;
    //vec3 radiance = light_lhtValues.color * attenuation * 0.75;

    return radiance;
}

float Distribution(vec3 halfwayVec, float roughness, vec3 normal)
{
    float a = pow(roughness, 2);
    float a2 = pow(a, 2);
    float NdotH = max(dot(normal, halfwayVec), 0.0);
    float NdotH2 = pow(NdotH, 2);

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * pow(denom, 2);

    return num / denom;
}

float GSchlickGGX(float NdotCamDir, float roughness)
{
    float r = (roughness + 1.0);
    float k = pow(r, 2) / 8.0;

    float num   = NdotCamDir;
    float denom = NdotCamDir * (1.0 - k) + k;

    return num / denom;
}

float Geometry(vec3 camDir, vec3 lightDir, float roughness, vec3 normal)
{
    float NdotCamDir = max(dot(normal, camDir), 0.0);
    float NdotLightDir = max(dot(normal, lightDir), 0.0);
    float ggx2 = GSchlickGGX(NdotCamDir, roughness);
    float ggx1 = GSchlickGGX(NdotLightDir, roughness);

    return ggx1 * ggx2;
}

vec3 Fresnel(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 BRDF(float D, float G, vec3 F, vec3 camDir, vec3 lightDir, vec3 albedo, float metallic, vec3 normal)
{
    vec3 numerator    = D * G * F; 
    float denominator = 4.0 * max(dot(normal, camDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  

    return (kD * albedo / PI + specular);
}

void main() 
{
    vec3 albedo;
    vec3 normal;
    float metallic;
    float roughness;
    float ao;

    // Albedo
    if (per_bTexs.HasAlbedoTexture)
    {
        albedo = texture(per_albedoMap, inTexCoord).rgb;
    }
    else
    {
        albedo = per_texsVal.albedoTexture;
    }

    // Normal
    if (per_bTexs.HasNormalTexture)
    {
        normal = getNormalFromMap();
    }
    else
    {
        normal = inNormal;
    }

    // Metallic 
    if (per_bTexs.HasMetallicTexture)
    {
         metallic = texture(per_metallicMap, inTexCoord).r;
    }
    else
    {
       metallic = per_texsVal.metallicTexture;
    }

    // Roughness
    if (per_bTexs.HasRoughnessTexture)
    {
        roughness = texture(per_RoughnessMap, inTexCoord).r;
    }
    else
    {
        roughness = per_texsVal.roughnessTexture;
    }

    // AO
    if (per_bTexs.HasAoTexture)
    {
        ao = texture(per_aoMap, inTexCoord).r;
    }
    else
    {
        ao = per_texsVal.aoTexture;
    }
    
    vec3 camDir = normalize(inCamPos - inWorldPos);

     vec3 F0 = vec3(0.04); 
     F0 = mix(F0, albedo, metallic);


     vec3 Lo = vec3(0.0);

    for(int i = 0; i < 1; ++i) 
    {
        vec3 lightDir = normalize(light_lhtValues.position - inWorldPos);
        vec3 halfwayVec = normalize(camDir + lightDir);
        
        vec3 radiance = Radiance(lightDir);

        float D   = Distribution(halfwayVec, roughness, normal);   
        float G   = Geometry(camDir, lightDir, roughness, normal);      
        vec3 F    = Fresnel(max(dot(halfwayVec, camDir), 0.0), F0);
           
        vec3 brdf = BRDF(D, G, F, camDir, lightDir, albedo, metallic, normal);

        float cosTheta = max(dot(normal, lightDir), 0.0);        

        Lo += brdf * radiance * cosTheta;
    }   
    
    vec3 ambient = vec3(0.03) * albedo * ao;
    //vec3 ambient = vec3(0.03) * albedo;
    vec3 color = ambient + Lo;
    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));

    //outColor = vec4(color, 1.0 );
    outColor = vec4(color, 1.0 );
}
