// Toon pixel shader

Texture2D shaderTexture : register(t0);
Texture2D shaderTexture1 : register(t1);
SamplerState sampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightPosition;
    float padding;
};

cbuffer DualBuffer : register(b1)
{
    bool dual;
}

struct InputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{        
    float4 textureColour;
    float3 lightVector;
    float lightDot;
    
    // Get normalized light vector compared to world position
    lightVector = normalize(lightPosition - input.position3D);
    
    // Get normalized dot product between object normal and light
    lightDot = max(0, dot(input.normal, lightVector));
    
    // Sample the pixel color from the textures using the sampler at the texture coordinate location
    textureColour = shaderTexture.Sample(sampleType, input.tex);
    ///if (dual) { textureColour *= shaderTexture1.Sample(sampleType, input.tex); }
    
    // Calculate toon shading threshold based on light
    if (lightDot > 0.9f)
    {
        textureColour *= 1.f;
    }
    else if (lightDot > 0.65f)
    {
        textureColour *= 0.8f;
    }
    else if (lightDot > 0.4f)
    {
        textureColour *= 0.6f;
    }
    else if (lightDot > 0.15f)
    {
        textureColour *= 0.4f;
    }
    else
    {
        textureColour *= 0.2f;
    }
    
    return textureColour;
}

