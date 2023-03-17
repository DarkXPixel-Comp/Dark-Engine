#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 1
#endif

#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 1
#endif

#ifndef NUM_SPOT_LIGHTS
    #define NUM_SPOT_LIGHTS 0
#endif


#include "LightingUtils.hlsl"


struct PSInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : WNORMAL;
};


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
	float gZNear;
	float4 gAmbientLight;
	float gFarZ;
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
	//return input.color + gAmbientLight;
	
	input.normalW = normalize(input.normalW);
	
	float3 toEye = normalize(gEyePos - input.posW);
	
	float4 ambient = gAmbientLight * gDiffuse;
	

	const float shininess = 1.0f - gRoughness;
	Material mat = { gDiffuse, gFrensel, shininess };
	float3 shadowFactor = 1.0f;
	float4 directLight = ComputeLighting(gLights, mat, input.posW, input.normalW, toEye, shadowFactor);
	
	
	float4 litColor = ambient + directLight;
	litColor.a = gDiffuse.a;
	//return ambient;
	return litColor;

}