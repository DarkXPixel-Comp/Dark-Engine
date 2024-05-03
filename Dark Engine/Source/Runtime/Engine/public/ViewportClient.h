#pragma once

#include "DarkClient.h"
#include "Engine/World.h"
#include "Math/MathFwd.h"


class FViewport;
class FCanvas;

class FViewportClient
{
public:
	FViewportClient() {}
	virtual ~FViewportClient() {}

	virtual void RedrawRequested(FViewport* Viewport) {}
	virtual void Draw(FViewport* InViewport, FCanvas* Canvas) {}

	virtual void Tick(float DeltaTime) {}

	virtual bool InputKey() { return false; }

	virtual FWorld* GetWorld() const { return nullptr; }

	void SetSize(FIntPoint InSize) { SizeClient = InSize; }
	

protected:
	FIntPoint SizeClient;
	//FRenderTarget* RenderTarget;

	




 };