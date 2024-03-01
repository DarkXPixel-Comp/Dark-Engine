#pragma once

#include "DarkClient.h"
#include "World.h"
#include "Math/MathFwd.h"


class FViewport;

class FViewportClient
{
public:
	FViewportClient() {}
	virtual ~FViewportClient() {}

	virtual void RedrawRequested(FViewport* Viewport) {}
	virtual void Draw(FViewport* InViewport, FWorld* InWorld) {}

	virtual void Tick(float DeltaTime) {}

	virtual bool InputKey() { return false; }

	virtual FWorld* GetWorld() const { return nullptr; }

	void SetSize(FIntPoint InSize) { SizeClient = InSize; }
	

protected:
	FIntPoint SizeClient;
	//FRenderTarget* RenderTarget;

	




 };