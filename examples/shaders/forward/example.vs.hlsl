struct PSInput
{
    float4 position : SV_POSITION;
    float2 tc : TEXCOORD0;
};

struct PushConstants {
    float displacement;
};

[[vk::push_constant]] ConstantBuffer<PushConstants> pushConstants : register(b16);

PSInput main(float3 position : POSITION, float2 tc : TEXCOORD0)
{
    PSInput result;
    
    result.position = float4(position.x + pushConstants.displacement, position.y, position.z, 1.0f);
    result.tc = tc;

    return result;
}