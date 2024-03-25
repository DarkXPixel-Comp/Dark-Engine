#pragma once
#include "DarkClient.h"
#include "Rendering/RenderingCommon.h"
#include "Widgets/UIViewport.h"



class UIViewport;


class FSceneViewport : public FViewport, public IUIViewport
{
public:
	FSceneViewport(FViewportClient* InViewportClient, UIViewport* InViewportWidget) {}
	virtual FUIShaderResource* GetViewportRenderTargetTexture() const { return nullptr; }
	virtual FIntPoint GetSize() const { return GetSizeXY(); }
	virtual void Resize(int32 InSizeX, int32 InSizeY);


	void InitRHI();



private:
	FIntPoint RTTSize;
	TRefCountPtr<FRHITexture> BufferedRenderTargetRHI;
	TRefCountPtr<FRHITexture> BufferedShaderResourceTextureRHI;


};