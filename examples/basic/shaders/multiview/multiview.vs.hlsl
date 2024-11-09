struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

PSInput main(float3 position : POSITION, uint view : SV_VIEWID)
{
    PSInput result = (PSInput)0;
    result.position = float4(position.x, position.y, position.z, 1.0f);
    result.color = (view == 0 ? float4(1.0f, 1.0f, 0.0f, 1.0f) : float4(0.0f, 0.0f, 1.0f, 1.0f)); // Red for view 0, blue for view 1
    return result;
}