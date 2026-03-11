struct PSInput
{
    float4 position : SV_POSITION;
};

struct PushConstants
{
    uint2 buffer;
};
ConstantBuffer<PushConstants> pushConstants : register(b0);

PSInput main(float3 position : POSITION)
{
    PSInput result = (PSInput) 0;
    return result;
}
