// Texture pixel shader

// Maximum number of lights
#define MAXLIGHTS 3

// Light types
#define DIRECTIONAL 1
#define POINT 2
#define SPOT 3

Texture2D shaderTexture : register(t0);
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

cbuffer LightBuffer : register(b0)
{
    //float4 ambientColor;
    //float4 diffuseColor;
    //float3 lightPosition;
    //float padding;
    Light Lights[MAXLIGHTS];
};

struct InputType
{
    float4 position : SV_POSITION;    
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float3 position3D : TEXCOORD2;
};

// Calculate lighting intensity from direction & normal, then combine with light colour
float4 CalculateLighting(float3 direction, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, direction));
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
    return CalculateLighting(light.direction, normal, light.diffuseColour) + light.ambientColour;
}

// Calculate point
float4 CalculatePoint(Light light, float3 normal, float3 vec)
{
    float attenuation = CalculateAttenuation(light.constA, light.linA, light.quadA, vec);
    return (CalculateLighting(vec, normal, light.diffuseColour) * attenuation) + light.ambientColour;
}

// Calculate spot
float4 CalculateSpot(Light light, float3 normal, float3 vec)
{
    float attenuation = CalculateAttenuation(light.constA, light.linA, light.quadA, vec);
    float intensity = CalculateCone(light.spotAngle, light.direction, vec);
    return (CalculateLighting(vec, normal, light.diffuseColour) * attenuation * intensity) + light.ambientColour;
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
        if (Lights[i].enabled < 0.5f) { continue; }
        
        if (Lights[i].type == 0)
        {
            col += CalculateDirectional(Lights[i], normal);
        }
        if (Lights[i].type == 1)
        {
            vec = normalize(Lights[i].lightPosition - position3D);
            col += CalculatePoint(Lights[i], vec, normal);
        }
        if (Lights[i].type == 2)
        {
            vec = normalize(Lights[i].lightPosition - position3D);
            col += CalculateSpot(Lights[i], vec, normal);
        }
    }
    
    return col;
}

float4 main(InputType input) : SV_TARGET
{    
 //   float4 textureColor;
 //   float3 lightDir;
 //   float lightIntensity;
 //   float4 color;

	//// Invert the light direction for calculations.
 //   lightDir = normalize(input.position3D - lightPosition);

	//// Calculate the amount of light on this pixel.
	//lightIntensity = saturate(dot(input.normal, -lightDir));

	//// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
 //   color = ambientColor + (diffuseColor * lightIntensity); //adding ambient
 //   color = saturate(color);

	//// Sample the pixel color from the textures using the sampler at these texture coordinate location.
 //   textureColor = shaderTexture.Sample(sampleType, input.tex);
 //   color *= textureColor;
    
 //   return color;
    
    // Sample texture
    float4 textureColour = shaderTexture.Sample(sampleType, input.tex);

    // Add lighting
    textureColour *= GenerateLight(input.normal, input.position3D);
    
    return textureColour;
}

