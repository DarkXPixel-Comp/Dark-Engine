#pragma once
#include "Platform/Platform.h"

struct EVisibility
{
	static DARKUI_API const EVisibility Visible;

	static DARKUI_API const EVisibility Collapsed;

	static DARKUI_API const EVisibility Hidden;

	static DARKUI_API const EVisibility HitTestInvisible;

	static DARKUI_API const EVisibility SelfHitTestInvisible;

	static DARKUI_API const EVisibility All;

public:
	EVisibility() : Visibility(VIS_Visible) {}


	bool operator==(const EVisibility& Other) const
	{
		return Visibility == Other.Visibility;
	}

	bool AreChildrenHitTestVisible() const
	{
		return Visibility & VISPRIVATE_ChildrenHitTestVisible;
	}

	bool IsHitTestVisible() const
	{
		return Visibility & VISPRIVATE_SelfHitTestVisible;
	}

	bool IsVisible() const
	{
		return Visibility & VIS_Visible;
	}

	static FORCEINLINE bool DoesVisibilityPassFilter(const EVisibility InVisibility, const EVisibility InVisibilityFilter)
	{
		return InVisibility.Visibility & InVisibilityFilter.Visibility;
	}

private:
	enum Private : uint8
	{
		VISPRIVATE_Visible = 0x1 << 0,
		VISPRIVATE_Collapsed = 0x1 << 1,
		VISPRIVATE_Hidden = 0x1 << 2,
		VISPRIVATE_SelfHitTestVisible = 0x1 << 3,
		VISPRIVATE_ChildrenHitTestVisible = 0x1 << 4,


		VIS_Visible = VISPRIVATE_Visible | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible,
		VIS_Collapsed = VISPRIVATE_Collapsed,
		VIS_Hidden = VISPRIVATE_Hidden,
		VIS_HitTestInvisible = VISPRIVATE_Visible,
		VIS_SelfHitTestInvisible = VISPRIVATE_Visible | VISPRIVATE_ChildrenHitTestVisible,


		VIS_All = VISPRIVATE_Visible | VISPRIVATE_Hidden | VISPRIVATE_Collapsed | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible
	};

	EVisibility(Private InValue)
		: Visibility(InValue)
	{
	}

	Private Visibility;

};