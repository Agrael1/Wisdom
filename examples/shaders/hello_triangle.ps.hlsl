struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

float4 main(PSInput input) : SV_TARGET0
{
    return input.color;
}
