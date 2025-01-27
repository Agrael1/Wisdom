struct Payload
{
    float3 color;
    bool allowReflection;
    bool missed;
};

[[vk::binding(0,1)]] RWTexture2D<float4> uav[] : register(u0);
[[vk::binding(0,2)]] RaytracingAccelerationStructure scene[] : register(t0, space1);


static const float3 camera = float3(0, 0, -5);
static const float3 light = float3(0, 200, 0);
static const float3 skyTop = float3(0.24, 0.44, 0.72);
static const float3 skyBottom = float3(0.75, 0.86, 0.93);

[shader("raygeneration")]
void RayGeneration()
{
    uint2 idx = DispatchRaysIndex().xy;
    float2 size = DispatchRaysDimensions().xy;

    float2 uv = idx / size;
    uv = uv * 2 - 1;


    RayDesc ray;
    ray.Origin = float3(uv.x, uv.y, camera.z);
    ray.Direction = float3(uv.x, uv.y, 1);
    ray.TMin = 0.001;
    ray.TMax = 1000;

    Payload payload;
    payload.allowReflection = true;
    payload.missed = false;

    TraceRay(scene[0], RAY_FLAG_NONE, 0xFF, 0, 0, 0, ray, payload);

    uav[0][idx] = float4(payload.color, 1);

}

[shader("miss")]
void Miss(inout Payload payload)
{

    float slope = normalize(WorldRayDirection()).y;
    float t = saturate(slope * 5 + 0.5);
    payload.color = lerp(skyBottom, skyTop, t);

    payload.missed = true;
}

[shader("closesthit")]
void ClosestHit(inout Payload payload,
                BuiltInTriangleIntersectionAttributes attrib)
{
    payload.color = float3(1, 1, 0);
}
