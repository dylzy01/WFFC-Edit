// Texture vertex shader

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float3 position3D : TEXCOORD2;
};

OutputType main(InputType input)
{
    OutputType output;
    
    // Change position vector to be 4 units for proper matrix calculations
    input.position.w = 1.f;
    
    // Calculate position of vertex against world, view and projection matrices
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Calculate normal vector against world matrix only & normalize
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    
    // Store texture coordinates for pixel shader
    output.tex = input.tex;
    
    // Calculate the world position of the vertex, for the point light
    output.position3D = (float3) mul(input.position, worldMatrix);
    
    return output;
}