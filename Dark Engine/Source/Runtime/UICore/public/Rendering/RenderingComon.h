#pragma once
#include "CoreMinimal.h"
#include "UIShaderResource.h"



class IUIViewport
{
public:
	virtual void OnDrawViewport() {}
	virtual FVector2D GetSize() const = 0;

	virtual FUIShaderResource* GetViewportRenderTargetTexture() const = 0;

	virtual void OnMouseEnter() {}
	virtual void OnMouseLeave() {}


	




};