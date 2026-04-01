struct PSInput {
    float4 position : SV_POSITION;
};
struct PSOutput {
    float4 color : SV_TARGET0;
};

PSOutput main(PSInput input)
{
    PSOutput output = (PSOutput)0;
    output.color = float4(0,0.5,0.3,1);
    return output;
}
