#pragma once

#include "DarkClient.h"
#include "World.h"


class FViewport;

class FViewportClient
{
public:
	FViewportClient() {}
	virtual ~FViewportClient() {}

	virtual void RedrawRequested(FViewport* Viewport);
	virtual void Draw(FViewport* InViewport, FWorld* InWorld) {}

	virtual void Tick(float DeltaTime) {}

	virtual bool InputKey();


	virtual FWorld* GetWorld() const { return nullptr; }


	




 };