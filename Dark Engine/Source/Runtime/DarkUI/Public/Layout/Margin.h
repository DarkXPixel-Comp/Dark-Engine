#pragma once
#include "CoreTypes.h"

#include "Math/Vector2.h"

struct FMargin
{
	float Left;

	float Top;

	float Right;

	float Bottom;

	FMargin()
		: Left(0.f)
		, Top(0.f)
		, Right(0.f)
		, Bottom(0.f)
	{
	}

	FMargin(float InLeft, float InTop, float InRight, float InBottom)
		: Left(InLeft)
		, Top(InTop)
		, Right(InRight)
		, Bottom(InBottom)
	{
	}

	FMargin(float InLeft, float InTop)
		: Left(InLeft)
		, Top(InTop)
		, Right(InLeft)
		, Bottom(InTop)
	{
	}


	FMargin(float InAll)
		: Left(InAll)
		, Top(InAll)
		, Right(InAll)
		, Bottom(InAll)
	{
	}

	FMargin(const FVector2f& InAll)
		: Left(InAll.X)
		, Top(InAll.Y)
		, Right(InAll.X)
		, Bottom(InAll.Y)
	{
	}

	FMargin(const FVector2f& InLeftTop, const FVector2f& InRightBottom)
		: Left(InLeftTop.X)
		, Top(InLeftTop.Y)
		, Right(InRightBottom.X)
		, Bottom(InRightBottom.Y)
	{
	}

	FMargin operator*(const float Scale) const
	{
		return FMargin(Left * Scale, Top * Scale, Right * Scale, Bottom * Scale);
	}

	FMargin operator*(const FMargin& Scale) const
	{
		return FMargin(Left * Scale.Left, Top * Scale.Top, Right * Scale.Right, Bottom * Scale.Bottom);
	}
};