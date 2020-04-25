// Water pixel shader

Texture2D shaderTexture0 : register(t0);
Texture2D shaderTexture1 : register(t1);
Texture2D shaderTexture2 : register(t2);
SamplerState sampleType : register(s0);

cbuffer WaterBuffer
{
    float4 tint;
    float translation;
    float scale;
    float2 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float3 position3D : TEXCOORD0;
    float4 refractPosition : TEXCOORD1;
    float4 reflectPosition : TEXCOORD2;
    float3 direction : TEXCOORD3;
    float2 repeatX : TEXCOORD4;
    float2 repeatY : TEXCOORD5;
};

// Calculate repeating normal
float3 CalculateNormalRepeat(float2 rX, float2 rY)
{
    float4 nX, nY, normal;
    
    // Update position
    rX += translation;
    rY += translation;
    
    // Sample normal map
    nX = shaderTexture2.Sample(sampleType, rX);
    nY = shaderTexture2.Sample(sampleType, rY);

    // Clamp normal range
    nX.rgb = (nX.rgb * 2.f) - 1.f;
    nY.rgb = (nY.rgb * 2.f) - 1.f;
    
    // Normal combo
    normal = normalize(nX.rgb + nY.rgb);

    return normal;
}

// Calculate fresnal factor
float CalculateFresnal(float3 normal, float3 direction)
{
    // Calculate light ratio
    float ratio = (1.2f - 1.f) / (1.2f - 1.f);

    // Return fresnal factor
    return max(0.f, min(1.f, ratio + (1.f - ratio) * pow(1.f - dot(normal, direction), 2.f)));
}

// Calculate colour
float4 CalculateColour(float3 normal, float4 refractPosition, float4 reflectPosition, float3 direction)
{
    float2 tex;
    float4 refract, reflect;
    
    // Calculate refract coords
    tex.xy = float2(refractPosition.x / refractPosition.w / 2.f + 0.5f, refractPosition.y / refractPosition.w / 2.f + 0.5f);
    tex = tex + (normal.xy * scale);

    // Sample refraction colour with tint
    refract = shaderTexture0.Sample(sampleType, tex);
    refract = saturate(refract * tint);

    // Calculate reflect coords
    tex.xy = float2(reflectPosition.x / reflectPosition.w / 2.f + 0.5f, reflectPosition.y / reflectPosition.w / 2.f + 0.5f);
    tex = tex + (normal.xy * scale);

    // Sample reflection colour
    reflect = shaderTexture1.Sample(sampleType, tex);

    // Update direction to only use Y
    direction.x = direction.y;
    direction.y = direction.y;
    direction.z = direction.y;
    
    // Return combination of colours
    return lerp(reflect, refract, CalculateFresnal(normal, direction));

}

float4 main(InputType input) : SV_TARGET
{
    // Get repeating normal
    float3 repeat = CalculateNormalRepeat(input.repeatX, input.repeatY);

    // Return combo colours
    return CalculateColour(repeat, input.refractPosition, input.reflectPosition, input.direction);
}