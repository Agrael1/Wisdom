struct PSInput {
    float4 position : SV_POSITION;
};

struct PushConstants {
    uint buffer;
};
[[vk::push_constant]] ConstantBuffer<PushConstants> pushConstants : register(b0);

struct Offsets {
    float offset;
};

// binding 0, space 1 is used for samplers
// binding 0, space 2 is used for constant buffers
[[vk::binding(0, 2)]] ConstantBuffer<Offsets> offsets[] : register(b0, space2);

PSInput main(float3 position : POSITION, uint view : SV_VIEWID)
{
    PSInput result = (PSInput)0;

    float offset = offsets[pushConstants.buffer].offset;
    result.position = float4(position.x + offset, position.y, position.z, 1.0f);
    return result;
}
