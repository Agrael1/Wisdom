struct Payload
{
    float3 color;
    bool allowReflection;
    bool missed;
};

[[vk::binding(0,1)]] RWTexture2D<float4> uav[] : register(u0);
[[vk::binding(0,2)]] RaytracingAccelerationStructure scene[] : register(t0, space1);


static const float3 camera = float3(0, 1.5, -7);
static const float3 light = float3(0, 200, 0);
static const float3 skyTop = float3(0.24, 0.44, 0.72);
static const float3 skyBottom = float3(0.75, 0.86, 0.93);

[shader("raygeneration")]
void RayGeneration()
{

    uint2 idx = DispatchRaysIndex().xy;
    float2 size = DispatchRaysDimensions().xy;
    uav[0][idx] = float4(1,1,0, 1);

    //float2 uv = idx / size;
    //float3 target = float3((uv.x * 2 - 1) * 1.8 * (size.x / size.y),
    //                       (1 - uv.y) * 4 - 2 + camera.y,
    //                       0);

    //RayDesc ray;
    //ray.Origin = camera;
    //ray.Direction = target - camera;
    //ray.TMin = 0.001;
    //ray.TMax = 1000;

    //Payload payload;
    //payload.allowReflection = true;
    //payload.missed = false;

    //TraceRay(scene, RAY_FLAG_NONE, 0xFF, 0, 0, 0, ray, payload);

}

[shader("miss")]
void Miss(inout Payload payload)
{

    float slope = normalize(WorldRayDirection()).y;
    float t = saturate(slope * 5 + 0.5);
    payload.color = lerp(skyBottom, skyTop, t);

    payload.missed = true;
}

void HitCube(inout Payload payload, float2 uv);
void HitMirror(inout Payload payload, float2 uv);
void HitFloor(inout Payload payload, float2 uv);

[shader("closesthit")]
void ClosestHit(inout Payload payload,
                BuiltInTriangleIntersectionAttributes attrib)
{

    switch (InstanceID())
    {
        case 0:
            HitCube(payload, attrib.barycentrics);
            break;
        case 1:
            HitMirror(payload, attrib.barycentrics);
            break;
        case 2:
            HitFloor(payload, attrib.barycentrics);
            break;

        default:
            payload.color = float3(1, 0, 1);
            break;
    }
}

void HitCube(inout Payload payload, float2 uv)
{
    //uint tri = PrimitiveIndex();

    //tri /= 2;
    //float3 normal = (tri.xxx % 3 == uint3(0, 1, 2)) * (tri < 3 ? -1 : 1);

    //float3 worldNormal = normalize(mul(normal, (float3x3) ObjectToWorld4x3()));

    //float3 color = abs(normal) / 3 + 0.5;
    //if (uv.x < 0.03 || uv.y < 0.03)
    //    color = 0.25.xxx;

    //color *= saturate(dot(worldNormal, normalize(light))) + 0.33;
    //payload.color = color;
}

void HitMirror(inout Payload payload, float2 uv)
{
    //if (!payload.allowReflection)
    //    return;

    //float3 pos = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
    //float3 normal = normalize(mul(float3(0, 1, 0), (float3x3) ObjectToWorld4x3()));
    //float3 reflected = reflect(normalize(WorldRayDirection()), normal);

    //RayDesc mirrorRay;
    //mirrorRay.Origin = pos;
    //mirrorRay.Direction = reflected;
    //mirrorRay.TMin = 0.001;
    //mirrorRay.TMax = 1000;

    //payload.allowReflection = false;
    //TraceRay(scene, RAY_FLAG_NONE, 0xFF, 0, 0, 0, mirrorRay, payload);

}

void HitFloor(inout Payload payload, float2 uv)
{
    //float3 pos = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();

    //bool2 pattern = frac(pos.xz) > 0.5;
    //payload.color = pattern.x ^ pattern.y ? 0.6.xxx : 0.4.xxx;

    //RayDesc shadowRay;
    //shadowRay.Origin = pos;
    //shadowRay.Direction = light - pos;
    //shadowRay.TMin = 0.001;
    //shadowRay.TMax = 1;

    //Payload shadow;
    //shadow.allowReflection = false;
    //shadow.missed = false;
    //TraceRay(scene, RAY_FLAG_NONE, 0xFF, 0, 0, 0, shadowRay, shadow);

    //if (!shadow.missed)
    //    payload.color /= 2;
}

