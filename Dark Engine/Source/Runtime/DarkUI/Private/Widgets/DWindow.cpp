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

void DUIWindow::SetCachedSize(FVector2f NewSize)
{
	if (NativeWindow)
	{

	}

}
