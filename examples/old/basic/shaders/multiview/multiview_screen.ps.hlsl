struct PSInput {
    float4 position : SV_POSITION;
    float2 tc : TEXCOORD0;
};
struct PSOutput {
    float4 color : SV_TARGET0;
};

struct PushConstants {
    uint frame;
};
[[vk::push_constant]] ConstantBuffer<PushConstants> pushConstants : register(b0);


[[vk::binding(0, 1)]] Texture2DArray tex[] : register(t0, space1);
[[vk::binding(0, 2)]] SamplerState sampler_point[] : register(s0, space2);

PSOutput main(PSInput ps_in)
{
    PSOutput output;

    // transform the texture coordinates to the correct layer
    float layer = ps_in.tc.x > 0.5f ? 1 : 0;
    float3 uv = float3(ps_in.tc * float2(2, 1) - layer * float2(1, 0), layer);

    output.color = tex[pushConstants.frame].Sample(sampler_point[0], uv);
    return output;
}
