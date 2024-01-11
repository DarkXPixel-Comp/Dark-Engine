#include "LightingUtils.hlsl"
#include "Common.hlsl"

cbuffer cbObject : register(b0)
{
	float4x4 ModelMatrix;
}

cbuffer cbPass : register(b3)
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




PSInput main(float3 position : SV_Position,
			float3 normal : NORMAL,
			float4 color : COLOR,
			float3 tangent : TANGENT,
			float3 bitangent : BITANGENT,
			float2 uv : TEXCOORD)
{
    PSInput result;
	
	float3 n = normal;
	
	float4 posW = mul(ModelMatrix, float4(position, 1.f));
	float4 pos = mul(gViewProj, posW);
	//pos.z += -0.1;
	result.position = pos;
	result.posW = posW;
	result.color = (color);
	float3 normalW = mul((float3x3) ModelMatrix, normal);
	result.normalW = normal;
	
	result.tangentW = mul((float3x3) ModelMatrix, tangent);
	
    result.uv = uv;
    result.normal = normal;

    return result;
}

