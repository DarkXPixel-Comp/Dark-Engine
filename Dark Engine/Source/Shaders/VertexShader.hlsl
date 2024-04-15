#include "LightingUtils.hlsl"
#include "Common.hlsl"

cbuffer cbObject : register(b0)
{
	float4x4 ModelMatrix;
}

cbuffer cbPass : register(b1)
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




PSInput main(float3 position : Attribute0,
			float3 normal : Attribute1,
			float4 color : Attribute2,
			float3 tangent : Attribute3,
			float3 bitangent : Attribute4,
			float2 uv : Attribute5)
{
    PSInput result;
	
	float3 n = normal;
	
	float4 posW = mul(ModelMatrix, float4(position, 1.f));
	float4 pos = mul(gViewProj, posW);
//	pos.z += -0.1;
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

