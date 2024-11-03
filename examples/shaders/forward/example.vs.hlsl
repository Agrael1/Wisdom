struct PSInput
{
    float4 position : SV_POSITION;
    float2 tc : TEXCOORD0;
};

struct PushConstants {
    float displacement[2];
};

[[vk::push_constant]] ConstantBuffer<PushConstants> pushConstants : register(b16);

PSInput main(float3 position : POSITION, float2 tc : TEXCOORD0, uint view : SV_VIEWID)
{
    PSInput result;
    
    float displacement = pushConstants.displacement[view];

    result.position = float4(position.x + displacement, position.y, position.z, 1.0f);
    result.tc = tc;

    return result;
}