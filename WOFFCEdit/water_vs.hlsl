// Water vertex shader

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix reflectionMatrix;
};

cbuffer WaterBuffer : register(b1)
{
    float3 cameraPosition;
    float2 normalRepeat;
    float2 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 refractPosition : TEXCOORD0;
    float4 reflectPosition : TEXCOORD1;
    float3 direction : TEXCOORD2;
    float2 repeatX : TEXCOORD3;
    float2 repeatY : TEXCOORD4;
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
    
    // Calculate refraction position of vertex against world, view and projection matrices
    output.refractPosition = mul(input.position, worldMatrix);
    output.refractPosition = mul(output.position, viewMatrix);
    output.refractPosition = mul(output.position, projectionMatrix);
    
    // Calculate reflection position of vertex against world, view and projection matrices
    output.reflectPosition = mul(input.position, worldMatrix);
    output.reflectPosition = mul(output.position, viewMatrix);
    output.reflectPosition = mul(output.position, projectionMatrix);
    
    // Calculate world position of the vertex
    float3 position3D = (float3) mul(input.position, worldMatrix);
    
    // Calculate direction
    output.direction = cameraPosition - position3D;
    output.direction = normalize(output.direction);
    
    // Calculate tex coords for water simulation
    output.repeatX = input.tex / normalRepeat.x;
    output.repeatY = input.tex / normalRepeat.y;
    
    return output;
}