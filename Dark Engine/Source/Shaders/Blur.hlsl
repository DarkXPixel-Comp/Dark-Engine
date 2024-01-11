SamplerState gSamPointWrap : register(s0);
SamplerState gSamPointClamp : register(s1);
SamplerState gSamLinearWrap : register(s2);
SamplerState gSamLinearClamp : register(s3);
SamplerState gSamAnisotropicWrap : register(s4);
SamplerState gSamAnisotropicClamp : register(s5);

Texture2D tRender : register(t0);




struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};



float4 main(PSInput input) : SV_TARGET
{
	const float offset[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
	const float weight[] =
	{
		0.2270270270, 0.1945945946, 0.1216216216,
  0.0540540541, 0.0162162162
	};
	
	float2 dir = float2(1.f, 0.f);
	float4 ppColour = tRender.Sample(gSamPointWrap, input.uv) * weight[0];
	float4 FragmentColor = float4(0.0f, 0.0f, 0.0f, 0.f);
	float hstep = dir.x;
	float vstep = dir.y;
	
	
	for (int i = 1; i < 5; i++)
	{
		FragmentColor += tRender.Sample(gSamPointWrap, input.uv + float2(hstep * offset[i], vstep * offset[i])) * weight[i] +
		tRender.Sample(gSamPointWrap, input.uv - float2(hstep * offset[i], vstep * offset[i])) * weight[i];
	}
	
	ppColour += FragmentColor;
	
	return ppColour;
	
}