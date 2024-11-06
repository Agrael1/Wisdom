struct PSInput
{
    float4 position : SV_POSITION;
    float2 tc : TEXCOORD0;
};
struct PSOutput {
    float4 color : SV_TARGET0;
};

[[vk::binding(0,0)]] Texture2D texture0 : register(t0);
[[vk::binding(0,1)]] SamplerState samLinear : register(s0);

PSOutput main(PSInput input)
{
    PSOutput output;
    output.color = texture0.Sample(samLinear, input.tc);
    return output;
}
