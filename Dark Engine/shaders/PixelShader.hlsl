struct PSInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer cbPass : register(b1)
{
	float4x4 gView;
	float4x4 gProj;
	float4x4 gViewProj;
	float2 gRenderTargetSize;
	float gTotalTime;
	float gDeltaTime;
	float4 gAmbientLight;
}



cbuffer cbMaterial : register(b2)
{
	float4 gDiffuse;
	float3 gFrensel;
	float gRoughness;
	float4x4 gMatTransform;
}




float4 main(PSInput input) : SV_TARGET
{
	return input.color + gAmbientLight;

}