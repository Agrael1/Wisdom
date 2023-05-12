struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct PSOutput
{
	float4 color : SV_TARGET0;
	float4 color2 : SV_TARGET1;
};

PSOutput main(PSInput input)
{
    PSOutput output;
    output.color = input.color;
    output.color2 = input.color;
    return output;
}