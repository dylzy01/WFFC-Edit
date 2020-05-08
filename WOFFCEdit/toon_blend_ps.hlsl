// Toon pixel shader - blend

#define MAX_COUNT 10

Texture2D shaderTexture : register(t0);
Texture2D shaderTexture1 : register(t1);
SamplerState sampleType : register(s0);

struct Light
{
    float4 diffuseColour;
    float4 ambientColour;
    float3 lightPosition;
    float spotAngle;
    
    float3 direction;
    float constA;
    float linA;
    float quadA;
    int type;
    float enabled;
};

cbuffer ActiveCountBuffer : register(b0)
{
    int activeLights;
};

cbuffer LightBuffer : register(b1)
{
    Light Lights[MAX_COUNT];
};

struct InputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float3 position3D : TEXCOORD2;
};

// Produce lighting
float4 GenerateLight(float3 normal, float3 position3D)
{
    // Initialise colour value
    float4 col = { 0.f, 0.f, 0.f, 0.f };
    
    // Loop through lights to determine pixel colour
    [unroll]
    for (int i = 0; i < activeLights; ++i)
    {
        // Continue if light is enabled
        if (Lights[i].enabled < 0.5f)
        {
            continue;
        }
        
        // Setup direction & distance
        float3 dir = Lights[i].lightPosition - position3D;
        float dis = length(dir);
        
        // Setup diffuse colour
        float4 diffuse = Lights[i].diffuseColour;
        
        // Apply different lighting based on type
        if (Lights[i].type == 1)
        {
            // Calculate attenuation
            float attenuation = 1.f / (Lights[i].constA + (Lights[i].linA + dis) + (Lights[i].quadA * pow(dis, 2)));
    
            // Apply attenuation to diffuse colour
            diffuse *= attenuation;
        }
        
        // Calculate lighting
        float light = saturate(dot(normal, normalize(dir)));
        col += saturate(diffuse * light);
        col += Lights[i].ambientColour;
    }
    
    return col;
}

float4 main(InputType input) : SV_TARGET
{
    // Sample textures
    float4 textureColour = shaderTexture.Sample(sampleType, input.tex);
    textureColour *= shaderTexture1.Sample(sampleType, input.tex);
    
    // Get normalized light vector compared to world position
    float3 direction = normalize(Lights[0].lightPosition - input.position3D);
    
    // Get normalized dot product between object normal and light
    float lightDot = max(0, dot(input.normal, direction));
    
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
    
    // Add lighting
    textureColour *= GenerateLight(input.normal, input.position3D);
    
    return textureColour;
}

