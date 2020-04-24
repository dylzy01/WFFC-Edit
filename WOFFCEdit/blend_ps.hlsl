Texture2D shaderTexture0 : register(t0);
Texture2D shaderTexture1 : register(t1);
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

float4 GetSlopeTexture(float2 tex, float3 normal, float3 worldPosition)
{
    float4 texture0, texture1;
    float4 slopeTexture;
    float4 textureColour;
    float slope;
    float blendAmount;
    
    // Sample first texture colour
    texture0 = shaderTexture0.Sample(sampleType, tex);

    // Sample second texture colour
    texture1 = shaderTexture1.Sample(sampleType, tex);
    
    // Set slope colour = second texture for blending
    slopeTexture = texture1;
    slopeTexture = saturate(slopeTexture);
    
    // Calculate slope
    slope = 1.f - normal.y;
    
    // Determine which texture to use based on steepness of slope
    if (slope < 0.05f)
    {
        blendAmount = slope / 0.05f;
        textureColour = lerp(texture0, slopeTexture, blendAmount);
    }
    else if ((slope < 0.6f) && (slope >= 0.05f))
    {
        blendAmount = (slope - 0.05f) * (1.f / (0.6f - 0.05f));
        textureColour = lerp(slopeTexture, texture1, blendAmount);
    }
    else if (slope >= 0.6f)
    {
        textureColour = texture1;
    }
    
   // Setup lighting
    textureColour *= ambientColor + diffuseColor;
    
    return textureColour;
}

float4 main(InputType input) : SV_TARGET
{
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
    textureColor0 = shaderTexture0.Sample(sampleType, input.tex);
    textureColor1 = shaderTexture1.Sample(sampleType, input.tex);
    color *= (textureColor0 * textureColor1);
    
    return color;
    
    //float4 colour = GetSlopeTexture(input.tex, input.normal, input.position);
    //colour = saturate(colour);
    //return colour;
}