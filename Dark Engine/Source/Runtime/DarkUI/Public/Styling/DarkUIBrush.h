#pragma once
#include "Styling/DarkUIColor.h"
#include "Math/Vector2.h"
#include "Layout/Margin.h"


enum EDUIBrushDrawType : uint8
{
	NoDrawType,

	Box,

	Border,

	Image,

	RoundedBox
};


enum EDUIBrushTileType : uint8
{
	NoTile,

	Horizontal,

	Vertical,

	Both
};


enum EDUIBrushMirrorType : uint8
{
	NoMirror,

	Horizontal,

	Vertical,

	Both
};

enum EDUIBrushImageType : uint8
{
	NoImage,

	FullColor,

	Linear,

	Vector
};


enum EDUIBrushRoundingType : uint8
{
	FixedRadius,

	HalfHeightRadius
};

struct FDUIBrush
{
public:
	DARKUI_API FDUIBrush();

	virtual ~FDUIBrush() {}

public:
	FDUIColor TintColor;

	EDUIBrushDrawType DrawType;

	EDUIBrushTileType TileType;

	EDUIBrushMirrorType MirrorType;

	EDUIBrushImageType ImageType;

	FVector2f ImageSize;

	FMargin Margin;






};