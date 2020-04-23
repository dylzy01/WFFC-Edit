// Texture pixel shader

Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightPosition;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;    
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{    
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

	// Invert the light direction for calculations.
    lightDir = normalize(input.position3D - lightPosition);

	// Calculate the amount of light on this pixel.
	///lightIntensity = saturate(dot(input.normal, -lightDir));
    lightIntensity = 1.f;

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    color = ambientColor + (diffuseColor * lightIntensity); //adding ambient
    color = saturate(color);

	// Sample the pixel color from the textures using the sampler at these texture coordinate location.
    textureColor = shaderTexture.Sample(sampleType, input.tex);
    color *= textureColor;
    
    return color;
}

