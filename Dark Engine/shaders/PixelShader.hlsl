Texture2D my_texture : register(t1);
SamplerState my_sampler : register(s0);





//
//cbuffer float4 LightColor: register(b2);
//float4 LightPos : register(b3);
//float4 CamPos : register(b4);
//matrix Model : register(b5);

//cbuffer Light
//{
//	float4 LightColor : register(b2);
//	float4 LightPos : register(b3);
//	float4 CamPos : register(b4);
//	matrix Model : register(b5);
//}

cbuffer lightpos : register(b2)
{
	float4 LightColor : packoffset(c);
}


cbuffer lightpos : register(b3)
{
	float4 LightPos;
}
cbuffer campos : register(b4)
{
	float4 CamPos;
}
cbuffer mvp : register(b5)
{
	matrix Model;
}


cbuffer time : register(b6)
{
	float iTime;
}







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
	//return input.color;



	float3 ambient = 0.1f * LightColor;


	

	float2 uv = input.uv;

	//uv *= cos(iTime) * 1;

	float3 FragPos = mul(Model, float4(input.posObj, 1.f));

	float3 norm = normalize(input.normal);
	float3 lightDir = normalize(LightPos.xyz - FragPos);


	float diff = max(dot(norm, lightDir), 0.f);
	float3 diffuse = diff * LightColor;

	
	float4 result = float4(ambient, 1.f) + float4(diffuse, 1.f);

	result.w = 1.f;


	return my_texture.Sample(my_sampler, uv) * result;

	//return pp;

	//return Sample * LightColor * (diffuse + ambient + specular);
	


  //return g_texture.Sample(g_sampler, input.uv);
	//return float4(1,0.5,0,0.5);
}