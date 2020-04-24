// Outline pixel shader

Texture2D shaderTexture : register(t0);
SamplerState outlineSampler : register(s0);
SamplerState textureSampler : register(s1);

cbuffer screen : register(b0)
{
    float screenHeight;
    float screenWidth;
    float2 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 depthPosition : TEXCOORD1;
    float edge_width : TEXCOORD2;
    float edge_intensity : TEXCOORD3;
    float normal_threshold : TEXCOORD4;
    float depth_threshold : TEXCOORD5;
    float normal_sensitivity : TEXCOORD6;
    float depth_sensitivity : TEXCOORD7;
};

float4 main(InputType input) : SV_TARGET
{
	// Sample initial render texture colour
    float3 textureColour = shaderTexture.Sample(textureSampler, input.tex);

	// Scale edge to screen size
    float2 edgeOffset = input.edge_width / float2(screenWidth, screenHeight);

	// Apply kernel to render texture
    float4 sobelOne = shaderTexture.Sample(outlineSampler, input.tex + float2(-1, -1) * edgeOffset);
    float4 sobelTwo = shaderTexture.Sample(outlineSampler, input.tex + float2(1, 1) * edgeOffset);
    float4 sobelThree = shaderTexture.Sample(outlineSampler, input.tex + float2(-1, 1) * edgeOffset);
    float4 sobelFour = shaderTexture.Sample(outlineSampler, input.tex + float2(1, -1) * edgeOffset);

	// Get normal and depth deltas
    float4 diagDelta = abs(sobelOne - sobelTwo) + abs(sobelThree - sobelFour);
    float nDelta = dot(diagDelta.xyz, 1.f);
    float dDelta = diagDelta.w;
	
	// Apply thresholds/sensitivities to deltas
    nDelta = saturate((nDelta - input.normal_threshold) * input.normal_sensitivity);
    dDelta = saturate((dDelta - input.depth_threshold) * input.depth_sensitivity);

	//Calculate edge from delta and edge intensity
    float edge = saturate(nDelta + dDelta) * input.edge_intensity;
	
	// Output outlined render texture
    textureColour *= (1.f - edge);

    return float4(textureColour, 1.f);
}