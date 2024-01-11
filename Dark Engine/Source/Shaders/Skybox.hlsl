#include "LightingUtils.hlsl"

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 PosL : Position;
	//float4 outPos : SV_POSITION;
};


cbuffer cbPass : register(b0)
{
	Light gLights[16];
	float4x4 gView;
	float4x4 gProj;
	float4x4 gViewProj;
	float2 gRenderTargetSize;
	float gTotalTime;
	float gDeltaTime;
	float3 gEyePos;
	float gNearZ;
	float4 gAmbientLight;
	float gFarZ;
}


cbuffer cbObject : register(b1)
{
	float4x4 ModelMatrix;
}





PSInput main(float3 position : SV_Position, float2 uv : TEXCOORD)
{
	PSInput result;
	
	//float4 posW = mul(float4(position, 1.f), ModelMatrix);
	//result.pos = mul(posW, gViewProj).xyww;
	
	float4 posW = mul(ModelMatrix, float4(position, 1.f));
	//posW.xyz += gEyePos;
	float4 pos = mul(gViewProj, posW).xyww;

	
	result.pos = pos;
	result.uv = uv;
	result.PosL = position;
	
	return result;
}