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



float4 main(PSInput input) : SV_TARGET
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