cbuffer Transform : register(b0)
{
    matrix modelViewProj;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput main(float3 pos : Position, float3 n : Normal, float4 color : Color)
{
    PSInput vertexShaderOutput;

    vertexShaderOutput.position = mul(float4(pos, 1.0f), modelViewProj);

    // float shade = (0.5 * dot(n, float3(0.0f, 1.0f, 0.0f))) + 0.5;
    // vertexShaderOutput.color = color * shade;
    vertexShaderOutput.color = color;

    return vertexShaderOutput;
}
