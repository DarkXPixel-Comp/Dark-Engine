#pragma once
#include "DarkClient.h"
#include "Rendering/RenderingCommon.h"
#include "Widgets/UIViewport.h"



class UIViewport;


class FSceneViewport : public FViewport, public IUIViewport
{
public:
	FSceneViewport(FViewportClient* InViewportClient, UIViewport* InViewportWidget): FViewport(InViewportClient), RTTSize(0, 0) {}
	virtual FUIShaderResource* GetViewportRenderTargetTexture() const { return nullptr; }
	virtual FIntPoint GetSize() const { return GetSizeXY(); }
	virtual void Resize(int32 InSizeX, int32 InSizeY);

	void SetTexturePath(FString Path);


	void InitRHI();



private:
	FIntPoint RTTSize;
	TRefCountPtr<FRHITexture> BufferedRenderTargetRHI;
	TRefCountPtr<FRHITexture> BufferedShaderResourceTextureRHI;
};