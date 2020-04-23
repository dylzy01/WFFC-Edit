Texture2D shaderTexture0 : register(t0);
Texture2D shaderTexture1 : register(t1);
SamplerState sampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

float4 main(InputType input) : SV_TARGET
{
    float4 colours[2];
    float4 blend;
    
    colours[0] = shaderTexture0.Sample(sampleType, input.tex);
    colours[1] = shaderTexture1.Sample(sampleType, input.tex);
    
    blend = colours[0] * colours[1] * 2.f;
    blend = saturate(blend);
    
    return blend;
}