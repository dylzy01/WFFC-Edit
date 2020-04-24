// Outline vertex shader

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer OutlineBuffer : register(b1)
{
    float edge_width;
    float edge_intensity;
    float normal_threshold;
    float depth_threshold;
    float normal_sensitivity;
    float depth_sensitivity;
    float2 padding;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
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

OutputType main(InputType input)
{
    OutputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Store normal
    output.normal = input.normal;

	// Store position for depth calculations
    output.depthPosition = output.position;

	// Store outline modifiers
    output.edge_width = edge_width;
    output.edge_intensity = edge_intensity;
    output.normal_threshold = normal_threshold;
    output.depth_threshold = depth_threshold;
    output.normal_sensitivity = normal_sensitivity;
    output.depth_sensitivity = depth_sensitivity;

    return output;
}