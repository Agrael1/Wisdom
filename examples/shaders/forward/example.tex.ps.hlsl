[[vk::binding(0, 0)]] Texture2D tex;
[[vk::binding(0, 1)]] SamplerState splr;


float4 main(float4 position : SV_POSITION, float2 tc : Texcoord) : SV_Target
{
    return tex.Sample(splr, tc);
}