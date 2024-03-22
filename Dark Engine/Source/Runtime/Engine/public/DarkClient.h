#pragma once
#include "ViewportClient.h"
#include "RHIResources.h"
#include "Memory/TUniquePtr.h"
#include "Math/IntPoint.h"






class FRenderTarget
{
public:
	virtual const TRefCountPtr<FRHITexture>& GetRenderTargetTexture() const;

	virtual FIntPoint GetSizeXY() const = 0;

	virtual float GetDisplayGamma() const { return 0; };






private:
	TRefCountPtr<FRHITexture> RenderTargetTexture;
};




class FViewport	 : public FRenderTarget
{
public:
	void Draw(FViewportClient* InViewport);

	FViewportClient* GetClient() const { return ViewportClient; }

	void SetSize(int32 InSizeX, int32 InSizeY) { SizeX = InSizeX; SizeY = InSizeY; }
	//FIntPoint GetSize() const { return FIntPoint(SizeX, SizeY); }
	FIntPoint GetSizeXY() const { return FIntPoint(SizeX, SizeY); }
	virtual float GetDisplayGamma() const override { return 0; }
	virtual void Resize(int32 InSizeX, int32 InSizeY) = 0;


private:
	FViewportClient* ViewportClient;
	int32 SizeX;
	int32 SizeY;


};