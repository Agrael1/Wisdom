struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

struct PushConstants
{
    float4 rotation;
};

[[vk::push_constant]] PushConstants g_push_constants;

VSOutput main(uint vertexId : SV_VertexID)
{
    VSOutput output = (VSOutput)0;

    const float2 positions[3] = {
        float2(0.0, 0.5),
        float2(0.5, -0.5),
        float2(-0.5, -0.5)
    };

    const float3 colors[3] = {
        float3(1.0, 0.0, 0.0),
        float3(0.0, 1.0, 0.0),
        float3(0.0, 0.0, 1.0)
    };

    const uint idx = min(vertexId, 2u);
    const float2 p = positions[idx];

    const float c = g_push_constants.rotation.x;
    const float s = g_push_constants.rotation.y;
    const float2 rotated = float2(c * p.x - s * p.y, s * p.x + c * p.y);

    output.position = float4(rotated, 0.0, 1.0);
    output.color = float4(colors[idx], 1.0);
    return output;
}
