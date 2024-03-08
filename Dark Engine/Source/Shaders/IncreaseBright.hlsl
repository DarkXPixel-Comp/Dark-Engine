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
	//return float4(0, 0, 0, 0);
	
	float4 col = tRender.Sample(gSamPointWrap, input.uv);
	
	//randFloat();
	
	col += cos(input.pos.x) * sin(input.pos.y) * 0.1;
	
	col.r += 0.05;
	
	return col;
}