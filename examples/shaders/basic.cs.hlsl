
struct Particle
{
    float4 position_life;
    float4 velocity_seed;
};

cbuffer SimParams : register(b0)
{
    uint  particleCount;
    uint  frameIndex;
    uint  dtMillis;
    uint  reset;
};

RWStructuredBuffer<Particle> particleBuffer : register(u0);

float Hash01(uint x)
{
    x ^= x * 0x27d4eb2d;
    x ^= x >> 15;
    x *= 0x85ebca6b;
    x ^= x >> 13;
    return frac((float)x / 65535.0);
}

[numthreads(256, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    const uint index = id.x;
    if (index >= particleCount) return;

    Particle p = particleBuffer[index];

    if (frameIndex == 0 || reset != 0 || p.position_life.w <= 0.0)
    {
        const float rx = Hash01(index * 17 + 3);
        const float ry = Hash01(index * 31 + 11);
        const float rv = Hash01(index * 47 + 19);

        p.position_life.xyz = float3(lerp(-0.9, 0.9, rx), 0.9 + 0.4 * ry, 0.0);
        p.position_life.w = 1.0;

        p.velocity_seed.xyz = float3(0.0, -(0.2 + rv * 0.8), 0.0);
        p.velocity_seed.w = rx;
    }

    const float dt = max(0.001, (float)dtMillis / 1000.0);
    p.position_life.xyz += p.velocity_seed.xyz * dt;

    if (p.position_life.y < -1.2)
    {
        p.position_life.y = 1.2;
    }

    particleBuffer[index] = p;
}
