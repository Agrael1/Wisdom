struct PSQuadIn {
    float2 texcoord : TexCoord;
    float4 position : SV_Position;
};

[[vk::binding(0, 0)]] Texture2D tex[2] : register(t0);
[[vk::binding(0, 1)]] SamplerState sampler_tex : register(s0);

float4 main(PSQuadIn ps_in)
    : SV_Target0
{
    float layer = ps_in.texcoord.x > 0.5f ? 1:0;
    float2 uv = ps_in.texcoord * float2(2, 1) - layer * float2(1, 0);
    float4 color = tex[layer].Sample(sampler_tex, uv);
    return color;
}
