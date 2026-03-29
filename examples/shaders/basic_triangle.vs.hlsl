struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

VSOutput main(uint vertexId : SV_VertexID)
{
    VSOutput output = (VSOutput)0;

    const float2 positions[3] = {
        float2( 0.0,  0.5),
        float2( 0.5, -0.5),
        float2(-0.5, -0.5)
    };

    const float3 colors[3] = {
        float3(1.0, 0.0, 0.0),
        float3(0.0, 1.0, 0.0),
        float3(0.0, 0.0, 1.0)
    };

    uint idx = min(vertexId, 2u);
    output.position = float4(positions[idx], 0.0, 1.0);
    output.color = float4(colors[idx], 1.0);
    return output;
}
