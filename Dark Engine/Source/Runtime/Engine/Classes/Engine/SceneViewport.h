#pragma once
#include "DarkClient.h"
#include "Rendering/RenderingCommon.h"
#include "Widgets/UIViewport.h"



class UIViewport;


class FSceneViewport : public FViewport, public IUIViewport
{
public:
	FSceneViewport(FViewportClient* InViewportClient, TSharedPtr<UIViewport> InViewportWidget);
	virtual FIntPoint GetSize() const { return GetSizeXY(); }
	virtual FUIShaderResource* GetViewportRenderTargetTexture() const { return nullptr; }

};