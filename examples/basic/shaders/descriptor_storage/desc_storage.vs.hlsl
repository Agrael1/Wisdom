struct PSInput {
    float4 position : SV_POSITION;
};

struct PushConstants {
    uint2 buffer;
};
[[vk::push_constant]] ConstantBuffer<PushConstants> pushConstants : register(b0);

struct OffsetX {
    float offset;
};

struct OffsetY {
    float offset;
};

[[vk::binding(0, 1)]] ConstantBuffer<OffsetX> offsetsx[] : register(b0, space1);
[[vk::binding(0, 1)]] ConstantBuffer<OffsetY> offsetsy[] : register(b0, space2); //overlap with space1

PSInput main(float3 position : POSITION)
{
    PSInput result = (PSInput)0;

    float offsetx = offsetsx[pushConstants.buffer.x].offset;
    float offsety = offsetsy[pushConstants.buffer.y + pushConstants.buffer.x].offset;
    result.position = float4(position.x + offsetx, position.y + offsety, position.z, 1.0f);
    return result;
}
