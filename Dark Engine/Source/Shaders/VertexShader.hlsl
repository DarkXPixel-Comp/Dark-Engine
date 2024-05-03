#include "LightingUtils.hlsl"
#include "Common.hlsl"


float4 PSmain(PSInput input) : SV_TARGET
{
	return float4(1, 1, 1, 1);
	/*//return gColorMap.Sample(gSamLinearClamp, input.uv);
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
	return litColor;*/

}



PSInput VSmain(float1 test : Attribute0)
{
	PSInput Result;
	return Result;
//	PSInput result;
	
//	float3 n = normal;
	
//	float4 posW = mul(ModelMatrix, float4(position, 1.f));
//	float4 pos = mul(gViewProj, posW);
////	pos.z += -0.1;
//	result.position = pos;
//	result.posW = posW;
//	result.color = (color);
//	float3 normalW = mul((float3x3) ModelMatrix, normal);
//	result.normalW = normal;
	
	
	
//	result.tangentW = mul((float3x3) ModelMatrix, tangent);
	
//	result.uv = uv;
//	result.normal = normal;

//	return result;
}