#pragma once
#include "Math/Vector2.h"

class FDUILayoutTransform
{
public:
	FDUILayoutTransform(float InScale = 1.f, FVector2f InTranslation = FVector2f(0.f)) : Scale(InScale), Translation(InTranslation) {}

	FVector2f GetTranslation() const { return Translation; }

	float GetScale() const { return Scale; }

private:
	float Scale;
	FVector2f Translation;
};