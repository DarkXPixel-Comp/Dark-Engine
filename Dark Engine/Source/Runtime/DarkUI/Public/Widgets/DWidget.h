#pragma once
#include "Memory/MemoryCore.h"
#include "Widgets/DUIControlConstruction.h"
#include "Widgets/DWidgetMacros.h"
#include "Layout/Geometry.h"
#include "Layout/ArrangedChildren.h"

#include "Math/Rect.h"
#include "InvalidateWidgetReason.h"



class FPaintArgs;


class DUIWidget	: 	
	public FDUIControlledConstruction,
	public std::enable_shared_from_this<DUIWidget>
{
	DUI_DECLARE_WIDGET_API(DUIWidget, FDUIControlledConstruction, DARKUI_API);

	template<class WidgetType, typename RequiredArgsPayloadType>
	friend struct TSlateDecl;
public:
	DARKUI_API int32 Paint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FRect4f& MyCullingRect, int32 LayerId);

	DARKUI_API virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

	DARKUI_API void Invalidate(EInvalidateWidgetReason InvalidateReason);

	void MarkPrepassAsDirty() { bNeedsPrepass = true; }

	DARKUI_API void OnToolTipClosing();

	FORCEINLINE bool IsVolatile() const { return bCachedVolatile; }

	FORCEINLINE void CacheVoltatility()
	{
		bCachedVolatile = bForceVolatile || ComputeVolatility();
	}

	DARKUI_API virtual void SetVisibility(TAttribute<EVisibility> InVisibility);
	FORCEINLINE EVisibility GetVisibility() const { return Visibility.Get(); }

	DARKUI_API virtual void ArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren, bool bInUpdateAttributes) const;

protected:
	DARKUI_API void DUIWidgetConstruct(const FDarkUIBaseNamedArgs& Args);

	bool IsConstructed() const { return false; }

	FORCEINLINE bool _InvalidateVolatility();

	virtual bool ComputeVolatility() const { return false; }


private:
//	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry) const = 0;

	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const {}

private:
	bool bNeedsPrepass = false;
	bool bCachedVolatile = true;
	bool bForceVolatile = true;

	TAttribute<EVisibility> Visibility;




};