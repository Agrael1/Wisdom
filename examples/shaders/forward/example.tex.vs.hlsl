struct PSInput
{
    float4 position : SV_POSITION;
    float2 tc : TEXCOORD0;
};

PSInput main(float3 pos : POSITION, float3 tc : TEXCOORD0)
{
    PSInput vertexShaderOutput;

    vertexShaderOutput.position = float4(pos, 1.0f);
    vertexShaderOutput.tc = tc.xy;

    return vertexShaderOutput;
}
