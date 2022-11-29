struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 posObj : POSITION;
};



float4 main(PSInput input) : SV_TARGET
{
	return input.color;




}
