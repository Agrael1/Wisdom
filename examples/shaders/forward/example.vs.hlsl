struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct PushConstants {
    float displacement;
};

[[vk::push_constant]] ConstantBuffer<PushConstants> pushConstants : register(b16);

PSInput main(float3 position : POSITION, float4 color : COLOR)
{
    PSInput result;
    
    result.position = float4(position.x + pushConstants.displacement, position.y, position.z, 1.0f);
    result.color = color;

    return result;
}