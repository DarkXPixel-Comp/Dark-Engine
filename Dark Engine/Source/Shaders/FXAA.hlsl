#include "Common.hlsl"

struct FBindlessIndexes
{
	uint InputTexture;
	uint OutputTexture;
};

ConstantBuffer<FBindlessIndexes> BindlessIndexes : register(b0);


groupshared float2 ScreenSize = float2(1.f, 1.f);
groupshared float2 InvScreenSize = float2(-1.f, -1.f);

float rgb2luma(float3 rgb)
{
    return sqrt(dot(rgb, float3(0.299, 0.587, 0.114)));
}


[numthreads(32, 32, 1)]
void FXAAMain
(
    uint3 DispatchThreadID : SV_DispatchThreadID, //<- Global position
    uint3 GroupThreadID : SV_GroupThreadID, //<- Group position
    uint GroupInDRx : SV_GroupIndex,
    uint GroupID : SV_GroupID
)
{
    float TexWidth;
    float TexHeigth;
    float2 OrigUV = (DispatchThreadID.xy + float2(0.5f, 0.5f)) * InvScreenSize;
    Texture2D InputTexture = ResourceDescriptorHeap[BindlessIndexes.InputTexture];
    Texture2D OutputTexture = ResourceDescriptorHeap[BindlessIndexes.OutputTexture];

    float2 ScreenPos = DispatchThreadID.xy;

    GroupMemoryBarrierWithGroupSync();

    if(ScreenPos.x >= ScreenSize.x || ScreenPos.y >= ScreenSize.y)
    {
        return;
    }

    float3 ColorCenter = InputTexture.SampleLevel(GetGlobalSampler(Point, Wrapped), OrigUV, 0.f).rgb;

    float LumaC = rgb2luma(ColorCenter);
    
    float LumaD = rgb2luma(InputTexture.SampleLevel(GetGlobalSampler(Point, Wrapped), OrigUV + float2(0.f, InvScreenSize.y), 0).rgb);
    float LumaU = rgb2luma(InputTexture.SampleLevel(GetGlobalSampler(Point, Wrapped), OrigUV + float2(0.f, -InvScreenSize.y), 0).rgb);
    float LumaL = rgb2luma(InputTexture.SampleLevel(GetGlobalSampler(Point, Wrapped), OrigUV + float2(-InvScreenSize.x, 0.f), 0).rgb);
    float LumaR = rgb2luma(InputTexture.SampleLevel(GetGlobalSampler(Point, Wrapped), OrigUV + float2(InvScreenSize.x, 0.f), 0).rgb);

    float LumaMin = min(LumaC, min(min(LumaD, LumaU), min(LumaL, LumaR)));
    float LumaMax = max(LumaC, max(max(LumaD, LumaU), max(LumaL, LumaR)));

    float LumaRange = LumaMax - LumaMin;
    float4 Result = float4(1.f, 1.f, 1.f, 1.f);



	
}