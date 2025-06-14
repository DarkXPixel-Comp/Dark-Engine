#include "Widgets/DWindow.h"	
#include "Misc/AssertionMacros.h"
#include "Platform/PlatformApplicationMisc.h"
#include "Application/DApplication.h"
#include "Math/Vector2.h"

void DUIWindow::Construct(const FArguments& InArgs)
{
	this->Title = InArgs._Title;
	this->Type = InArgs._Type;
	this->UserResizeBorder = InArgs._UserResizeBorder;
	this->SizingRule = InArgs._SizingRule;
	this->bInitialMaximize = InArgs._IsInitiallyMaximized;
	this->bInitialMinimize = InArgs._IsInitiallyMinimized;
	this->bShouldPreserveAspectRation = InArgs._ShouldPreserveAspectRatio;
	this->SizeLimits
		.SetMaxHeight(InArgs._MaxHeight)
		.SetMaxWidth(InArgs._MaxWidth)
		.SetMinHeight(InArgs._MinHeight)
		.SetMinWidth(InArgs._MinWidth);
	FVector2f WindowPosition = InArgs._ScreenPosition;

	if (InArgs._AdjustInitialSizeAndPositionForDPIScale && WindowPosition != FVector2f::ZeroVector)
	{
		float InitialDPIScale = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(WindowPosition.X, WindowPosition.Y);
		WindowPosition *= InitialDPIScale;
	}
	
	FDisplayMetrics DisplayMetrics;
	FDUIApplication::Get().GetCachedDisplayMetrics(DisplayMetrics);


	InitialDesiredSize = InArgs._Size;
	InitialDesiredScreenPosition = WindowPosition;
	bHasOSBorder = InArgs._bHasOSBorder;
	bIsRegularWindow = InArgs._bIsRegularWindow;


	//
	Size = InitialDesiredSize;


}


void DUIWindow::ResizeWindowSize(FVector2f NewWindowSize)
{


}


void DUIWindow::SetNativeWindow(const TSharedPtr<FGenericWindow>& InNativeWindow)
{
	NativeWindow = InNativeWindow;
}

FVector2f DUIWindow::GetInitialDesiredSizeInScreen() const
{
	return InitialDesiredSize;
}

FVector2f DUIWindow::GetInitialDesiredPositionInScreen() const
{
	return InitialDesiredScreenPosition;
}

void DUIWindow::ShowWindow()
{
	if (!bHasEverBeenShown)
	{
		InitialMaximize();

		InitialMinimize();
	}

	bHasEverBeenShown = true;

	if (NativeWindow)
	{
		NativeWindow->Show();

	}
}

DARKUI_API TArray<TSharedPtr<DUIWindow>>& DUIWindow::GetChildWindows()
{
	return ChildWindows;
}

DARKUI_API TSharedPtr<FGenericWindow> DUIWindow::GetNativeWindow() const
{
	return NativeWindow;
}

DARKUI_API bool DUIWindow::IsSubWindowOf(const TSharedPtr<DUIWindow>& ParentWindow) const
{
	if (ParentWindow.get() == this)
	{
		return true;
	}

	TSharedPtr<DUIWindow> ParentWnd = GetParentWindow();

	while (ParentWnd)
	{
		if (ParentWnd == ParentWindow)
		{
			return true;
		}

		ParentWnd = ParentWnd->GetParentWindow();
	}
	return false;
}

DARKUI_API TSharedPtr<DUIWindow> DUIWindow::GetParentWindow() const
{
	return ParentWindow.lock();
}

DARKUI_API EWindowZone DUIWindow::GetWindowZoneForPoint(const FVector2f& Point) const
{
	const bool bIsFullscreen = GetWindowMode() == EWindowMode::FullScreen || GetWindowMode() == EWindowMode::WindowdFullscreen;
	const bool bIsBorderlessGameWindow = Type == EWindowType::GameWindow && !bHasOSBorder;

	const float DPIScale = FDUIApplication::Get().GetApplicationScale() * GetDPIScale();

	const FMargin DPIScaledResizeBorder = UserResizeBorder * DPIScale;

	//const bool bIsCursorVisible = 

	//if((bIsFullscreen && !bIsBorderlessGameWindow)

	if (Point.X >= 0 && Point.X < Size.X &&
		Point.Y >= 0 && Point.Y < Size.Y)
	{
		int32 Row = 1;
		int32 Column = 1;

		if (SizingRule == ESizingRule::UserSize && !bIsFullscreen && !NativeWindow->IsMaximize())
		{
			if (Point.X < (DPIScaledResizeBorder.Left + 5))
			{
				Column = 0;
			}
			else if (Point.X >= Size.X - (DPIScaledResizeBorder.Right + 5))
			{
				Column = 2;
			}

			if (Point.Y < (DPIScaledResizeBorder.Top + 5))
			{
				Row = 0;
			}
			else if (Point.Y >= Size.Y - (DPIScaledResizeBorder.Bottom + 5))
			{
				Row = 2;
			}

			bool bInBorder = Point.X < DPIScaledResizeBorder.Left ||
				Point.X >= Size.X - DPIScaledResizeBorder.Right ||
				Point.Y < DPIScaledResizeBorder.Top ||
				Point.Y >= Size.Y - DPIScaledResizeBorder.Bottom;

			if (!bInBorder)
			{
				Row = 1;
				Column = 1;
			}
		}

		static const EWindowZone WindowZones[3][3] =
		{
			{ EWindowZone::TopLeftBorder, EWindowZone::TopBorder, EWindowZone::TopRightBorder },
			{ EWindowZone::LeftBorder, EWindowZone::ClientArea, EWindowZone::RightBorder },
			{ EWindowZone::BottomLeftBorder, EWindowZone::BottomBorder, EWindowZone::BottomRightBorder }
		};

		EWindowZone Zone = WindowZones[Row][Column];
		if (Zone == EWindowZone::ClientArea)
		{
		}
		WindowZone = Zone;
	}
	else
	{
		WindowZone = EWindowZone::NotInWindow;
	}

	return WindowZone;

}

float DUIWindow::GetDPIScale() const
{
	return NativeWindow ? NativeWindow->GetDPIScale() : 1.0f;
}

bool DUIWindow::IsVisible() const
{
	return false;//return NativeWindow ? NativeWindow->IsVisile
}

bool DUIWindow::AcceptsInput() const
{
	return Type != EWindowType::CursorDecorator && (Type != EWindowType::Tool || !FDUIApplication::Get().IsWindowHousingInteractiveTooltip(this->shared_from_this()));
}

bool DUIWindow::IsScreenSpaceMouseWithin(FVector2d MousePosition) const
{
	const FVector2f LocalMouseCoord = MousePosition - ScreenPosition;
	return !LocalMouseCoord.ConatinsNaN() && NativeWindow->IsPointInWindow(static_cast<int32>(LocalMouseCoord.X), static_cast<int32>(LocalMouseCoord.Y));
}

FVector2d DUIWindow::GetSize() const
{
	return Size;
}

FVector2d DUIWindow::GetPosition() const
{
	return ScreenPosition;
}

FVector2d DUIWindow::GetInitialDesiredSize() const
{
	return InitialDesiredSize;
}

FVector2d DUIWindow::GetInitialDesiredPosition() const
{
	return InitialDesiredScreenPosition;
}

bool DUIWindow::HasOSBorder() const
{
	return bHasOSBorder;
}

bool DUIWindow::IsRegularWindow() const
{
	return bIsRegularWindow;
}

bool DUIWindow::ShouldPreserveAspectRation() const
{
	return bShouldPreserveAspectRation;
}

void DUIWindow::SetCachedSize(FVector2f NewSize)
{
	if (NativeWindow)
	{
		FVector2d NewSize2D = NewSize;
		NativeWindow->AdjustSize(NewSize2D);
		NewSize = NewSize2D;
	}
	if (Size != NewSize)
	{
		Size = NewSize;
	}

}

void DUIWindow::SetCachedPosition(const FVector2f NewPosition)
{
	ScreenPosition = NewPosition;
}

FWindowSizeLimits DUIWindow::GetSizeLimits() const
{
	return SizeLimits;
}

void DUIWindow::InitialMinimize()
{
	if (NativeWindow && bInitialMinimize)
	{
		NativeWindow->Minimize();
	}
}

void DUIWindow::InitialMaximize()
{
	if (NativeWindow && bInitialMaximize)
	{
		NativeWindow->Maximize();
	}
}
