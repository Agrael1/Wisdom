struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
    float4 offset = float4(0, 0, 0, 0);
};

PSInput main(float3 position : POSITION, float4 color : COLOR)
{
    PSInput result;

    result.position = float4(position + offset.xyz, 1.0f);
    result.color = color;

    return result;
}