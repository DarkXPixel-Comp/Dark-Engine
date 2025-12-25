#pragma once
#include "Styling/DarkUIColor.h"
#include "Math/Vector2.h"
#include "Layout/Margin.h"


enum class EDUIBrushDrawType : uint8
{
	NoDrawType,

	Box,

	Border,

	Image,

	RoundedBox
};


enum class EDUIBrushTileType : uint8
{
	NoTile,

	Horizontal,

	Vertical,

	Both
};


enum class EDUIBrushMirrorType : uint8
{
	NoMirror,

	Horizontal,

	Vertical,

	Both
};

enum class EDUIBrushImageType : uint8
{
	NoImage,

	FullColor,

	Linear,

	Vector
};


enum class EDUIBrushRoundingType : uint8
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