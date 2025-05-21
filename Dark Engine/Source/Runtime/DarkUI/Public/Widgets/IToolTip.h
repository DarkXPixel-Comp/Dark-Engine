#pragma once
#include "CoreTypes.h"
#include "Memory/MemoryCore.h"
#include "Math/Vector2.h"


class DUIWindow;

class IToolTip
{
public:
	virtual TSharedPtr<DUIWindow> AsWidget() = 0;

	virtual TSharedPtr<DUIWindow> GetContentWidget() = 0;

	virtual void SetContentWidget(const TSharedPtr<DUIWindow>& InContentWidget) = 0;

	virtual void ResetContentWidget() {}

	virtual bool IsEmpty() const = 0;

	virtual bool IsInteractive() const = 0;

	virtual void OnOpening() = 0;

	virtual void OnClosed() = 0;

	virtual void OnSetInteractiveWindowLocation(FVector2d& InOutDesiredLocation) const {}


	virtual ~IToolTip() {}
};