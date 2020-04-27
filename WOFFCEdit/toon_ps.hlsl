// Toon pixel shader

// Maximum number of lights
#define NUMLIGHTS 3

// Light types
#define DIRECTIONAL 1
#define POINT 2
#define SPOT 3

Texture2D shaderTexture : register(t0);
Texture2D shaderTexture1 : register(t1);
SamplerState sampleType : register(s0);

struct Light
{
    float4  diffuseColour;
    float4  ambientColour;
    float3  lightPosition;
    float   spotAngle;
    
    float3  direction;
    float   constA;
    float   linA;
    float   quadA;
    int     type;
    float   enabled;
};

cbuffer LightBuffer : register(b0)
{
    Light   Lights[NUMLIGHTS];
};

struct InputType
{
    float4  position : SV_POSITION;
    float3  normal : NORMAL;
    float2  tex : TEXCOORD0;
    float3  position3D : TEXCOORD2;
};

// Calculate lighting intensity from direction & normal, then combine with light colour
float4 CalculateLighting(float3 direction, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, direction));
    ///intensity *= 2.f;
    return saturate(diffuse * intensity);
}

// Calculate attenuation
float CalculateAttenuation(float constA, float linA, float quadA, float3 vec)
{
    float dist = length(vec);
    return 1.f / (constA + linA * dist + quadA * dist * dist);
}

// Calculate cone
float CalculateCone(float angle, float3 direction, float3 vec)
{
    float minCos = cos(angle);
    float maxCos = (minCos + 1.f) / 2.f;
    float angleCos = dot(direction, vec);
    return smoothstep(minCos, maxCos, angleCos);
}

// Calculate directional
float4 CalculateDirectional(Light light, float3 normal)
{
    return CalculateLighting(light.direction, normal, light.diffuseColour);// + light.ambientColour);
}

// Calculate point
float4 CalculatePoint(Light light, float3 normal, float3 vec)
{
    float attenuation = CalculateAttenuation(light.constA, light.linA, light.quadA, vec);
    float4 diffuse = light.diffuseColour * attenuation;
    return light.ambientColour + CalculateLighting(normalize(vec), normal, diffuse);
}

// Calculate spot
float4 CalculateSpot(Light light, float3 normal, float3 vec)
{
    float attenuation = CalculateAttenuation(light.constA, light.linA, light.quadA, vec);
    float4 diffuse = light.diffuseColour * attenuation;
    float intensity = CalculateCone(light.spotAngle, light.direction, vec);
    return light.ambientColour + CalculateLighting(normalize(vec), normal, diffuse) * intensity;
}

// Produce lighting
float4 GenerateLight(float3 normal, float3 position3D)
{
    // Final colour & light vector
    float4 col = { 0.f, 0.f, 0.f, 0.f };
    float3 vec = { 0.f, 0.f, 0.f };

    // Loop through lights to determine pixel colour
    [unroll]
    for (int i = 0; i < 2; ++i)
    {
        if (Lights[i].enabled < 0.5f)
        {
            continue;
        }
        
        // Switch between light type
        //switch (Lights[i].type)
        //{
        //    case 1:
        //    {
        //        col += CalculateDirectional(Lights[i], normal);
        //        break;
        //    }
        //    case 2:
        //    {
        //        vec = normalize(Lights[i].lightPosition - position3D);
        //        col += CalculatePoint(Lights[i], vec, normal);
        //        break;
        //    }
        //    case 3:
        //    {
        //        vec = normalize(Lights[i].lightPosition - position3D);
        //        col += CalculateSpot(Lights[i], vec, normal);
        //        break;
        //    }
        //}
            
        if (Lights[i].type == 1)
        {
            col += CalculateDirectional(Lights[i], normal);
        }
        else if (Lights[i].type == 2)
        {
            vec = Lights[i].lightPosition - position3D;
            col += CalculatePoint(Lights[i], vec, normal);
        }
        else if (Lights[i].type == 3)
        {
            vec = Lights[i].lightPosition - position3D;
            col += CalculateSpot(Lights[i], vec, normal);
        }
    }
    
    return col;
}

float4 main(InputType input) : SV_TARGET
{        
    // Sample texture
    float4 textureColour = shaderTexture.Sample(sampleType, input.tex);    
    
    // Get distance between object and light
    float3 lightVector = Lights[0].lightPosition - input.position3D;
    float distance = length(lightVector);
    
    // Calculate attenuation
    ///float attenuation = 1.f / (Lights[0].constA + (Lights[0].linA + distance) + (Lights[0].quadA * pow(distance, 2)));
    ///float attenuation = 1.f / (1.f + (0.125f + distance) + (0.f * pow(distance, 2)));
    float attenuation = 1.f / (8.f + (0.125f + distance) + (5.f * pow(distance, 2)));
    float diffuse = Lights[0].diffuseColour * attenuation;
    
    // Get normalized light vector compared to world position
    lightVector = normalize(lightVector);
    float4 lightColour = Lights[0].ambientColour + CalculateLighting(lightVector, input.normal, diffuse);
    
    // Get normalized dot product between object normal and light
    float lightDot = max(0, dot(input.normal, lightVector));
    
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
    
    return textureColour * lightColour;
    
    // Sample texture
    //float4 textureColour = shaderTexture.Sample(sampleType, input.tex);
    
    //// Get normalized light vector compared to world position
    //float3 lightVector = normalize(Lights[0].lightPosition - input.position3D);
    
    //// Get normalized dot product between object normal and light
    //float lightDot = max(0, dot(input.normal, lightVector));
    
    //// Calculate toon shading threshold based on light
    //if (lightDot > 0.9f)
    //{
    //    textureColour *= 1.f;
    //}
    //else if (lightDot > 0.65f)
    //{
    //    textureColour *= 0.8f;
    //}
    //else if (lightDot > 0.4f)
    //{
    //    textureColour *= 0.6f;
    //}
    //else if (lightDot > 0.15f)
    //{
    //    textureColour *= 0.4f;
    //}
    //else
    //{
    //    textureColour *= 0.2f;
    //}     
    
    //// Add lighting
    //textureColour *= GenerateLight(input.normal, input.position3D);
    
    //return textureColour;
}