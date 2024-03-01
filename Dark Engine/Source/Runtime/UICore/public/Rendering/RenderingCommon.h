#pragma once
#include "CoreMinimal.h"
#include "UIShaderResource.h"



class IUIViewport
{
public:
	virtual void OnDrawViewport() {}
	virtual FIntPoint GetSize() const = 0;

	virtual FUIShaderResource* GetViewportRenderTargetTexture() const = 0;

	virtual void OnMouseEnter() {}
	virtual void OnMouseLeave() {}
};



//class FUIBackBuffer : public FRenderTarget
//{
//public:
//	FUIBackBuffer(TSharedPtr FRHITexture)
//};


//class FUIWindowViewport : public IUIViewport
//{
//public:
//	virtual FIntPoint GetSize() const { return FIntPoint(RenderTarget->GetWidth(), RenderTarget->GetHeight()); }
//	virtual FUIShaderResource* GetViewportRenderTargetTexture() const { return RenderTarget.get(); }
//
//
//
//
//private:
//
//};
//
