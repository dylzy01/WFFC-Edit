// Texture pixel shader

Texture2D shaderTexture0 : register(t0);
Texture2D shaderTexture1 : register(t1);
SamplerState SampleType : register(s0);

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
    //float4 colours[2];
    //float4 blend;
    
    //// Determine final amount of diffuse colour based on the diffuse colour combined with light intensity
    //blend = ambientColor + (diffuseColor * 2.f);
    //blend = saturate(blend);
    
    //// Get pixel colour from first texture
    //colours[0] = shaderTexture0.Sample(SampleType, input.tex);
    
    //// Get pixel colour from second texture
    //colours[1] = shaderTexture1.Sample(SampleType, input.tex);
    
    //// Blend the two pixels together and multiply by gamma value
    //blend *= (colours[0] * colours[1]);/// * 2.f;
    
    //// Saturate colour
    //blend = saturate(blend);
    
    //return blend;
    
    float4 textureColor0, textureColor1;
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
    textureColor0 = shaderTexture0.Sample(SampleType, input.tex);
    textureColor1 = shaderTexture1.Sample(SampleType, input.tex);
    color *= (textureColor0 * textureColor1);
    
    return color;
}

