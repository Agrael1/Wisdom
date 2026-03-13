struct Particle
{
    float4 position_life;
    float4 velocity_seed;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

struct FrameConstants
{
    uint frameIndex;
    uint particleCount;
    uint verticesPerParticle;
    uint reserved;
};

ConstantBuffer<FrameConstants> frameData : register(b0);
StructuredBuffer<Particle> particleBuffer : register(t0);

VSOutput main(uint vertexId : SV_VertexID)
{
    VSOutput result = (VSOutput)0;

    const uint particleIndex = vertexId / max(1u, frameData.verticesPerParticle);
    const uint cornerIndex = vertexId % max(1u, frameData.verticesPerParticle);

    const uint safeIndex = min(particleIndex, max(0u, frameData.particleCount - 1u));
    Particle p = particleBuffer[safeIndex];

    const float2 triOffsets[3] = {
        float2( 0.00,  0.03),
        float2(-0.02, -0.02),
        float2( 0.02, -0.02)
    };

    const float2 center = p.position_life.xy;
    const float2 pos = center + triOffsets[min(cornerIndex, 2u)];

    result.position = float4(pos, 0.0, 1.0);

    const float hue = frac(p.velocity_seed.w + frameData.frameIndex * 0.005);
    result.color = float4(0.3 + 0.7 * hue, 0.7 - 0.5 * hue, 1.0 - hue, 0.85);
    return result;
}
