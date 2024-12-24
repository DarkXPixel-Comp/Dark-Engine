#pragma once
#include "Memory/MemoryCore.h"
#include "Widgets/DUIControlConstruction.h"
#include "Widgets/DWidgetMacros.h"
#include "Layout/Geometry.h"

#include "Math/Rect.h"



class FPaintArgs;


class DUIWidget	: 	
	public FDUIControlledConstruction,
	public std::enable_shared_from_this<DUIWidget>
{
	DUI_DECLARE_WIDGET_API(DUIWidget, FDUIControlledConstruction, DarkUI_API);

	template<class WidgetType, typename RequiredArgsPayloadType>
	friend struct TSlateDecl;
public:
	DarkUI_API int32 Paint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FRect4f& MyCullingRect, int32 LayerId);

	DarkUI_API virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

protected:
	DarkUI_API void DUIWidgetConstruct(const FDarkUIBaseNamedArgs& Args);





};