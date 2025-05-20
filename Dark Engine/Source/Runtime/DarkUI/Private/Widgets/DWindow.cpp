#include "Widgets/DWindow.h"	
#include "Misc/AssertionMacros.h"
#include "Platform/PlatformApplicationMisc.h"
#include "Application/DApplication.h"
#include "Math/Vector2.h"

void DUIWindow::Construct(const FArguments& InArgs)
{
	this->Title = InArgs._Title;
	this->Type = InArgs._Type;
	FVector2f WindowPosition = InArgs._ScreenPosition;

	if (InArgs._AdjustInitialSizeAndPositionForDPIScale && WindowPosition != FVector2f::ZeroVector)
	{
		float InitialDPIScale = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(WindowPosition.X, WindowPosition.Y);
		WindowPosition *= InitialDPIScale;
	}
	


}


void DUIWindow::ResizeWindowSize(FVector2f NewWindowSize)
{


}

void DUIWindow::SetScreenPosition(const FVector2f& NewPosition)
{
	ScreenPosition = NewPosition;
}

void DUIWindow::SetSize(FVector2f NewSize)
{
	if (NativeWindow)
	{
		NativeWindow->AdjustSize(Size);
	}
	if (NewSize != Size)
	{
		Size = NewSize;
	}
	
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

DARKUI_API void DUIWindow::ShowWindow()
{
	if (!bHasEverBeenShown)
	{
		if (NativeWindow)
		{
			//FDUIApplication::

		}
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

	const float DPIScale = 



}

void DUIWindow::SetCachedSize(FVector2f NewSize)
{
	if (NativeWindow)
	{

	}

}
