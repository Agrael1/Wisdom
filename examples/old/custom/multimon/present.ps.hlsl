struct PSInput {
    float4 position : SV_POSITION;
    float2 tc : TEXCOORD0;
};
struct PSOutput {
    float4 color : SV_TARGET0;
};


[[vk::binding(0, 0)]] Texture2D tex : register(t0);
[[vk::binding(0, 1)]] SamplerState sampler_point : register(s0);

PSOutput main(PSInput ps_in)
{
    PSOutput output;
    output.color = tex.Sample(sampler_point, ps_in.tc);
    return output;
}
