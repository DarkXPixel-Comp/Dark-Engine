#pragma once
#include "Math/MathFwd.h"

class UIWidget
{
public:
	UIWidget() {}
	void Update(float DeltaTime) {}
	
	void SetSize(FIntPoint InSize) {}
	void SetPostion(FIntPoint InPosition) { Position = InPosition; }


protected:
	FIntPoint Position;
	FIntRect Rect;





};