#pragma once
#include "ViewportClient.h"


class FViewport
{
public:
	void Draw();

	FViewportClient* GetClient() const { return ViewportClient; }

	void SetSize(int32 InSizeX, int32 InSizeY) { SizeX = InSizeX; SizeY = InSizeY; }
	FIntPoint GetSize() const { return FIntPoint(SizeX, SizeY); }

private:
	FViewportClient* ViewportClient;
	int32 SizeX;
	int32 SizeY;


};