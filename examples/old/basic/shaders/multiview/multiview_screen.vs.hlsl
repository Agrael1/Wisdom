struct PSInput {
    float4 position : SV_POSITION;
    float2 tc : TEXCOORD0;
};

PSInput main(uint VertexID : SV_VertexID)
{ // ouputs a full screen quad with tex coords
    PSInput Out;
    Out.tc = float2((VertexID << 1) & 2, VertexID & 2);
    Out.position = float4(Out.tc * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    Out.tc = Out.tc;
    return Out;
}
