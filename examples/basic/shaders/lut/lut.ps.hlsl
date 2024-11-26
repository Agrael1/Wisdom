struct PSQuadIn {
    float2 texcoord : TexCoord;
    float4 position : SV_Position;
};

[[vk::binding(0, 0)]] Texture3D lut : register(t0);
[[vk::binding(1, 0)]] Texture2D tex : register(t1);
[[vk::binding(0, 1)]] SamplerState sampler_lut : register(s0);
[[vk::binding(1, 1)]] SamplerState sampler_tex : register(s1);

float3 Lut3DTetra(const float3 color)
{
    float3 dims;
    lut.GetDimensions(dims.x, dims.y, dims.z);

    // make cube
    float3 re_dims = dims - float3(1.0f, 1.0f, 1.0f);
    float3 restored = color * re_dims; // restore position in table

    float3 black = floor(restored);
    float3 white = ceil(restored);

    float3 fracts = frac(restored); // get normalized fractions

    float3 blackf = black / re_dims;
    float3 whitef = white / re_dims;

    // select tetrahedron
    bool3 cmp = fracts.rgb >= fracts.gbr; // (r>g, g>b, b>r)
    int res = min(int(cmp.x) * 4 + int(cmp.y) * 2 + int(cmp.z) - 1, 5);

    // variants:
    // r<g, g<b, b>r t2 (001) // B C          c.z  c.z + c.y
    // r>g, g<b, b<r t6 (010) // G Y  -1- G   c.y  c.y + c.x
    // r<g, g>b, b>r t3 (011) // G C          c.y  c.y + c.z
    // r>g, g<b, b<r t4 (100) // R M  1-- R   c.x  c.x + c.z
    // r>g, g<b, b>r t1 (101) // B M  --1 B   c.z  c.z + c.x
    // r>g, g>b, b<r t5 (110) // R Y          c.x  c.x + c.y

    float3 swizzle_rgb[6];
    swizzle_rgb[0] = float3(blackf.x, blackf.y, whitef.z);
    swizzle_rgb[1] = float3(blackf.x, whitef.y, blackf.z);
    swizzle_rgb[2] = float3(blackf.x, whitef.y, blackf.z);
    swizzle_rgb[3] = float3(whitef.x, blackf.y, blackf.z);
    swizzle_rgb[4] = float3(blackf.x, blackf.y, whitef.z);
    swizzle_rgb[5] = float3(whitef.x, blackf.y, blackf.z);

    float3 swizzle_cmy[6];
    swizzle_cmy[0] = float3(blackf.x, whitef.y, whitef.z);
    swizzle_cmy[1] = float3(whitef.x, whitef.y, blackf.z);
    swizzle_cmy[2] = float3(blackf.x, whitef.y, whitef.z);
    swizzle_cmy[3] = float3(whitef.x, blackf.y, whitef.z);
    swizzle_cmy[4] = float3(whitef.x, blackf.y, whitef.z);
    swizzle_cmy[5] = float3(whitef.x, whitef.y, blackf.z);

    float3 swizzle_xyz[6];
    swizzle_xyz[0] = float3(fracts.z, fracts.y, fracts.x);
    swizzle_xyz[1] = float3(fracts.y, fracts.x, fracts.z);
    swizzle_xyz[2] = float3(fracts.y, fracts.z, fracts.x);
    swizzle_xyz[3] = float3(fracts.x, fracts.z, fracts.y);
    swizzle_xyz[4] = float3(fracts.z, fracts.x, fracts.y);
    swizzle_xyz[5] = float3(fracts.x, fracts.y, fracts.z);

    float3 point_rgb = swizzle_rgb[res];
    float3 point_cmy = swizzle_cmy[res];
    float3 delta = swizzle_xyz[res];

    float4 s1 = lut.Sample(sampler_lut, blackf); // lower diagonal
    float4 s2 = lut.Sample(sampler_lut, point_rgb);
    float4 s3 = lut.Sample(sampler_lut, point_cmy);
    float4 s4 = lut.Sample(sampler_lut, whitef); // upper diagonal

    return ((1.0f - delta.x) * s1 + (delta.x - delta.y) * s2 + (delta.y - delta.z) * s3 + delta.z * s4).rgb;
}

float4 main(PSQuadIn ps_in)
    : SV_Target0
{
    float2 texcoord = ps_in.texcoord * float2(2, 1); // repeat texture
    float4 color = tex.Sample(sampler_tex, texcoord);

    // apply 3D LUT with tetrahedral interpolation
    float3 color3_lut = (texcoord.x > 1 ? Lut3DTetra(color.rgb) : color.rgb);
    return float4(color3_lut, color.a);
}
