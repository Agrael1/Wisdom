struct PSQuadIn
{
    float2 texcoord : TEXCOORD;
    float4 position : SV_POSITION;
};

Texture2D yTexture : register(t0);
Texture2D uvTexture : register(t1);
SamplerState sampler_tex : register(s0);

float4 main(PSQuadIn ps_in)
    : SV_TARGET0
{
    // Sample Y and UV components
    float y = yTexture.Sample(sampler_tex, ps_in.texcoord).r;
    float2 uv = uvTexture.Sample(sampler_tex, ps_in.texcoord).rg;

    // Convert YUV to RGB using BT.709 coefficients
    // Normalize Y from [16/255, 235/255] to [0, 1] for limited range
    y = (y - 16.0 / 255.0) * (255.0 / (235.0 - 16.0));

    // Normalize UV from [16/255, 240/255] to [-0.5, 0.5] for limited range
    uv = (uv - 128.0 / 255.0);

    float3 color;
    color.r = y + 1.5748 * uv.y; // V component
    color.g = y - 0.1873 * uv.x - 0.4681 * uv.y; // U and V components
    color.b = y + 2.03211 * uv.x; // U component

    return float4(saturate(color), 1.0);
}