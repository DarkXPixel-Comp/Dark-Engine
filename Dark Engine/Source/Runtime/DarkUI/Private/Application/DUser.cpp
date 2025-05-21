#include "Application/DUser.h"
#include "Widgets/DWidget.h"
#include "Widgets/IToolTip.h"


FDUIUser::FDUIUser(int32 InUserIndex) : UserIndex(InUserIndex)
{



}

DARKUI_API bool FDUIUser::IsWindowHousingInteractiveTooltip(const TSharedPtr<const DUIWindow>& WindowTest) const
{
	if (WindowTest == TooltipWindow.lock())
	{
		const TSharedPtr<IToolTip> ActiveTooltip = ActiveTooltipInfo.Tooltip.lock();
		return ActiveTooltip && ActiveTooltip->IsInteractive();
	}
	return false;
}

FDUIUser::~FDUIUser()
{
}

void FDUIUser::FActiveTooltipInfo::Reset()
{
	if (!SourceWidget.expired())
	{
		SourceWidget.lock()->OnToolTipClosing();
	}

	if (!Tooltip.expired())
	{
		Tooltip.lock()->OnClosed();
	}

	Tooltip.reset();
	SourceWidget.reset();
	TooltipVisualizer.reset();
	OffsetDirection = ETooltipOffsetDirection::Undetermined;
}
