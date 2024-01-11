SamplerState gSamPointWrap : register(s0);
SamplerState gSamPointClamp : register(s1);
SamplerState gSamLinearWrap : register(s2);
SamplerState gSamLinearClamp : register(s3);
SamplerState gSamAnisotropicWrap : register(s4);
SamplerState gSamAnisotropicClamp : register(s5);

TextureCube tSkybox : register(t2);




struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 PosL : Position;
	//float4 outPos : SV_POSITION;
};



float4 main(PSInput input) : SV_TARGET
{
	return tSkybox.Sample(gSamLinearWrap, input.PosL);
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}