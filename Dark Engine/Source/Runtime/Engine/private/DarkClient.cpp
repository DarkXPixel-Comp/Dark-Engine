#include "DarkClient.h"
#include "Canvas.h"


const TRefCountPtr<FRHITexture>& FRenderTarget::GetRenderTargetTexture() const
{
	return RenderTargetTextureRHI;
	
}


FIntRect FViewport::CalculateViewExtents(float AspectRation, const FIntRect& ViewRect)
{
	FIntRect Result = ViewRect;

	const float CurrentSizeX = ViewRect.RightDown.X - ViewRect.LeftUp.X;
	const float CurrentSizeY = ViewRect.RightDown.Y - ViewRect.LeftUp.Y;

	const float AdjustedAspectRation = AspectRation / ((((float)GetSizeXY().X / (float)GetSizeXY().Y)) / (float)GetSizeXY().X / (float)GetSizeXY().Y);
	
	const float AspectRationDifference = AdjustedAspectRation - (CurrentSizeX / CurrentSizeY);

	if (FMath::Abs(AspectRationDifference) > 0.01f)
	{
		if (AspectRationDifference > 0.f)
		{
			const int32 NewSizeY = FMath::Max(1, FMath::RoundToInt(CurrentSizeX / AdjustedAspectRation));
			Result.LeftUp.Y = FMath::RoundToInt(0.5f * (CurrentSizeY - NewSizeY));
			Result.RightDown.Y = Result.LeftUp.Y + NewSizeY;
			Result.LeftUp.Y += ViewRect.LeftUp.Y;
			Result.RightDown.Y += ViewRect.LeftUp.Y;
		}
		else
		{
			const int32 NewSizeX = FMath::Max(1, FMath::RoundToInt(CurrentSizeY / AdjustedAspectRation));
			Result.LeftUp.X = FMath::RoundToInt(0.5f * (CurrentSizeX - NewSizeX));
			Result.RightDown.X = Result.LeftUp.X + NewSizeX;
			Result.LeftUp.X += ViewRect.LeftUp.X;
			Result.RightDown.X += ViewRect.LeftUp.X;
		}
	}
	return Result;

}


FViewport::FViewport(FViewportClient* InViewportClient):
	ViewportClient(InViewportClient),
	SizeX(0),
	SizeY(0)
{

}

void FViewport::Draw()
{
	FCanvas Canvas(this, GetClient()->GetWorld());
	


	ViewportClient->Draw(this, &Canvas);

}