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
#include "Common.hlsl"


SamplerState gSamPointWrap : register(s0);
SamplerState gSamPointClamp : register(s1);
SamplerState gSamLinearWrap : register(s2);
SamplerState gSamLinearClamp : register(s3);
SamplerState gSamAnisotropicWrap : register(s4);
SamplerState gSamAnisotropicClamp : register(s5);

Texture2D tAlbedo : register(t0);
Texture2D tNormal : register(t1);


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
	//return gColorMap.Sample(gSamLinearClamp, input.uv);
	//return float4(1, 1, 1, 1);
	
	float4 gDiffuseAlbedo = gDiffuse * tAlbedo.Sample(gSamAnisotropicWrap, input.uv);
	float4 gNormalMapSample = tNormal.Sample(gSamAnisotropicWrap, input.uv);
	
	
	//return input.position.z ;
	
	float3 bumpedNormal = NormalSampleToWorldSpace(gNormalMapSample.xyz, input.normalW, input.tangentW);
	
	//bumpedNormal = input.normalW;
	

	input.normalW = normalize(input.normalW);
	
	float3 toEye = normalize(gEyePos - input.posW);
	
	float4 ambient = gAmbientLight * gDiffuseAlbedo;
	
	//return gDiffuse + 0;

	const float shininess = 1.0f - gRoughness;
	Material mat = { gDiffuseAlbedo, gFrensel, shininess };
	float3 shadowFactor = 1.0f;
	float4 directLight = ComputeLighting(gLights, mat, input.posW, bumpedNormal, toEye, shadowFactor);
	
	
	float4 litColor = ambient + directLight;
	
	litColor.a = gDiffuseAlbedo.a;
	//return ambient;
	return litColor;

}