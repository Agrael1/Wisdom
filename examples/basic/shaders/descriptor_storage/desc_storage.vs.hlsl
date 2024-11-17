#ifdef DXIL
#define DX_OFFSET(reg) (-reg)
#else // SPRIV
#define DX_OFFSET(reg) 0
#endif // DXIL


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

// binding 0, space 1 is used for samplers
// binding 0, space 2 is used for constant buffers
[[vk::binding(0, 2)]] ConstantBuffer<OffsetX> offsetsx[1] : register(b0, space2);
[[vk::binding(0, 2)]] ConstantBuffer<OffsetY> offsetsy[1] : register(b1, space2); 
// Note: different register, for DX this has to be different register
// for Vulkan it can be the same register without offset.
// That means we have to subtract the descriptor offset in the shader for DX compilation.

PSInput main(float3 position : POSITION)
{
    PSInput result = (PSInput)0;

    float offsetx = offsetsx[pushConstants.buffer.x + DX_OFFSET(0)].offset;
    float offsety = offsetsy[pushConstants.buffer.y + pushConstants.buffer.x + DX_OFFSET(1)].offset;
    result.position = float4(position.x + offsetx, position.y + offsety, position.z, 1.0f);
    return result;
}
