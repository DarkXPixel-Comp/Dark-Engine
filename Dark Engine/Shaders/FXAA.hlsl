#include "Common.hlsl"


#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define ITERATIONS 7
#define SUBPIXEL_QUALITY 0.75


groupshared static float QUALITY[7] = {1.5f, 2.f, 2.f, 2.f, 2.f, 4.f, 8.f};

struct FBindlessIndexes
{
	uint InputTexture;
	uint OutputTexture;
};

struct FParams
{
	uint2 ViewportSize;
};


ConstantBuffer<FBindlessIndexes> BindlessIndexes : register(b0);
ConstantBuffer<FParams> Params : register(b1);



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
	float2 ScreenSize = Params.ViewportSize;
	float2 InvScreenSize = 1.f / Params.ViewportSize;
    
    float TexWidth;
    float TexHeigth;
    float2 OrigUV = (DispatchThreadID.xy + float2(0.5f, 0.5f)) * InvScreenSize;
    Texture2D<float4> InputTexture = ResourceDescriptorHeap[BindlessIndexes.InputTexture];
	RWTexture2D<float4> OutputTexture = ResourceDescriptorHeap[BindlessIndexes.OutputTexture];

    float2 ScreenPos = DispatchThreadID.xy;

    GroupMemoryBarrierWithGroupSync();

    if(ScreenPos.x >= ScreenSize.x || ScreenPos.y >= ScreenSize.y)
    {
        return;
    }

    SamplerState Sampler = GetGlobalSampler(Point, Wrapped);
    float3 ColorCenter = InputTexture.SampleLevel(Sampler, OrigUV, 0.f).rgb;


    float LumaC = rgb2luma(ColorCenter);
    
    float LumaD = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(0.f, InvScreenSize.y), 0).rgb);
    float LumaU = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(0.f, -InvScreenSize.y), 0).rgb);
    float LumaL = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(-InvScreenSize.x, 0.f), 0).rgb);
    float LumaR = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(InvScreenSize.x, 0.f), 0).rgb);

    float LumaMin = min(LumaC, min(min(LumaD, LumaU), min(LumaL, LumaR)));
    float LumaMax = max(LumaC, max(max(LumaD, LumaU), max(LumaL, LumaR)));

    float LumaRange = LumaMax - LumaMin;
    float4 Result = float4(1.f, 1.f, 1.f, 1.f);

    if(LumaRange < max(EDGE_THRESHOLD_MIN, (LumaMax * EDGE_THRESHOLD_MAX)) || InvScreenSize.x == -1.f)
    {
        Result = float4(ColorCenter.xyz, 1.f);
    }
    else
    {
        float LumaDR = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(InvScreenSize.x, InvScreenSize.y), 0.f).rgb);
        float LumaDL = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(-InvScreenSize.x, InvScreenSize.y), 0.f).rgb);
        float LumaUR = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(InvScreenSize.x, -InvScreenSize.y), 0.f).rgb);
        float LumaUL = rgb2luma(InputTexture.SampleLevel(Sampler, OrigUV + float2(-InvScreenSize.x, -InvScreenSize.y), 0.f).rgb);


        float LumaDU = LumaD + LumaU;
        float LumaLR = LumaL + LumaR;

        float LumaRC = LumaUL + LumaDL;
        float LumaLC = LumaUR + LumaDR;
        float LumaDC = LumaDR + LumaDL;
        float LumaUC = LumaUR + LumaUL;

        float EdgeHor = abs(-2.f * LumaL + LumaLC) + abs(-2.f * LumaC + LumaDU) * 2.f + abs(-2.f * LumaR + LumaRC);
        float EdgeVer = abs(-2.f * LumaU + LumaUC) + abs(-2.f * LumaC + LumaLR) * 2.f + abs(-2.f * LumaD + LumaDC);

        bool bIsHorizontal = EdgeHor >= EdgeVer;


        float Luma1 = bIsHorizontal ? LumaD : LumaL;
        float Luma2 = bIsHorizontal ? LumaU : LumaR;

        float Grad1 = Luma1 - LumaC;
        float Grad2 = Luma2 - LumaC;

        bool bIsSteepest = abs(Grad1) >= abs(Grad2);

        float GradScaled = 0.25f * max(abs(Grad1), abs(Grad2));
        float StepLength = bIsHorizontal ? InvScreenSize.y : InvScreenSize.x;

        float LumaLocalAvg = 0.f;
        if(bIsSteepest)
        {
            StepLength = -StepLength;
            LumaLocalAvg = 0.5f * (Luma1 + LumaC);
        }
        else
        {
            LumaLocalAvg = 0.5f * (Luma2 + LumaC);
        }

        float2 CurrentUV = OrigUV;
        if(bIsHorizontal)
        {
            CurrentUV.y += StepLength * 0.5f;
        }
        else
        {
            CurrentUV.x += StepLength * 0.5f;
        }

     
        float2 Offset = bIsHorizontal ? float2(InvScreenSize.x, 0.f) : float2(0.f, InvScreenSize.y);
        float2 UV1 = CurrentUV - Offset;
        float2 UV2 = CurrentUV + Offset;

        float LumaEnd1 = rgb2luma(InputTexture.SampleLevel(Sampler, UV1, 0.f).rgb);
        float LumaEnd2 = rgb2luma(InputTexture.SampleLevel(Sampler, UV2, 0.f).rgb);

        bool Reached1 = abs(LumaEnd1) >= GradScaled;
        bool Reached2 = abs(LumaEnd2) >= GradScaled;

        if(!Reached1)
        {
            UV1 -= Offset;
        }
        if(!Reached2)
        {
            UV2 -= Offset;
        }

        if(!(Reached1 && Reached2))
        {
            for(int i = 2; i < ITERATIONS; ++i)
            {
                if(!Reached1)
                {
                    LumaEnd1 = rgb2luma(InputTexture.SampleLevel(Sampler, UV1, 0.f).rgb);
                    LumaEnd1 -= LumaLocalAvg;
                }
                if(!Reached1)
                {
                    LumaEnd2 = rgb2luma(InputTexture.SampleLevel(Sampler, UV2, 0.f).rgb);
                    LumaEnd2 -= LumaLocalAvg;
                }

                Reached1 = abs(LumaEnd1) >= GradScaled;
                Reached2 = abs(LumaEnd2) >= GradScaled;

                if(!Reached1)
                {
                    UV1 -= Offset * QUALITY[i];
                }
                if(!Reached2)
                {
                    UV2 += Offset * QUALITY[i];
                }
                if(Reached1 && Reached2)
                {
                    break;
                }
            }

        }

        float Dist1 = bIsHorizontal ? (OrigUV.x - UV1.x) : (OrigUV.y - UV1.y);
        float Dist2 = bIsHorizontal ? (UV2.x - OrigUV.x) : (UV2.y - OrigUV.y);

        bool bIsDir1 = Dist1 < Dist2;
        float FinalDist = min(Dist1, Dist2);

        float EdgeLength = Dist1 + Dist2;

        float PixelOffset = -FinalDist / EdgeLength + 0.5f;
        bool bIsLumaCSmaller = LumaC < LumaLocalAvg;
        bool bCorrectVariation = (((bIsDir1 ? LumaEnd1 : LumaEnd2) < 0.f) != bIsLumaCSmaller);

        float FinalOffset = bCorrectVariation ? PixelOffset : 0.f;

        float LumaAvg = (1.f / 12.f) * (2.f * (LumaDU + LumaLR) + LumaLC + LumaRC);

        float SubPixelOffset1 = saturate(abs(LumaAvg - LumaC) / LumaRange);
        float SubPixelOffset2 = (-2.f * SubPixelOffset1 + 3.f) * SubPixelOffset1 * SubPixelOffset1;

        float FinalSubPixelOffset = SubPixelOffset2 * SubPixelOffset2 * SUBPIXEL_QUALITY;

        FinalOffset = max(FinalOffset, FinalSubPixelOffset);

        float2 FinalUV = OrigUV;

        if(bIsHorizontal)
        {
            FinalUV.y += FinalOffset * StepLength;
        }
        else
        {
            FinalUV.x += FinalOffset * StepLength;
        }
        Result = InputTexture.SampleLevel(Sampler, FinalUV, 0.f);
    }


    OutputTexture[ScreenPos] = Result;

	
}