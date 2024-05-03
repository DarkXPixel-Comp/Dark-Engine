#pragma once
#include "DarkClient.h"


class FSceneInterface;


class FCanvas
{	
public:
	FCanvas(FRenderTarget* InRenderTarget, FWorld* InWorld);

	static FCanvas* Create();

public:
	FRenderTarget* GetRenderTarget() const { return RenderTarget; }
public:
	void Flush();

private:
	FRenderTarget* RenderTarget;
	FIntRect ViewRect;
	FIntRect ScissorRect;
	FSceneInterface* Scene;


};