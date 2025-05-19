#include "Widgets/DWindow.h"	
#include "Misc/AssertionMacros.h"
#include "Platform/PlatformApplicationMisc.h"

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

void DUIWindow::SetCachedSize(FVector2f NewSize)
{
	if (NativeWindow)
	{

	}

}
