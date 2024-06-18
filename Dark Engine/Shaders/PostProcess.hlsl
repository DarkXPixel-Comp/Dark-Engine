struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	//float4 outPos : SV_POSITION;
};


PSInput main(float3 pos : SV_Position, float2 uv : TEXCOORD)
{	
	
	PSInput output;
	//output.pos.xy = pos.xy;
	//output.pos.z = pos.z;
	//output.pos.w = 1;
	output.pos = float4(pos, 1);
	output.uv = uv;
	return output;
}
