#include "Application/DApplication.h"
#include "Widgets/DWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Platform/PlatformApplicationMisc.h"
#include "Application/DUser.h"
#include "Layout/ArrangedWidget.h"


TSharedPtr<FDUIApplication> FDUIApplication::CurrentApplication;
TSharedPtr<FGenericApplication> FDUIApplication::PlatformApplication;

void FDUIApplication::Create()
{
	Create(MakeShareble(FPlatformApplicationMisc::CreateApplication()));
}

DARKUI_API void FDUIApplication::Shutdown()
{
	FPlatformApplicationMisc::Shutdown();


}

TSharedPtr<FDUIApplication> FDUIApplication::Create(const TSharedPtr<FGenericApplication>& InPlatformApplication)  
{  
	PlatformApplication = InPlatformApplication;

	CurrentApplication = MakeShareble(new FDUIApplication());

	PlatformApplication->SetMessageHandler(CurrentApplication);

	{
		FDisplayMetrics DisplayMetrics;
		FDUIApplication::Get().GetDisplayMetrics(DisplayMetrics);
	}


	return CurrentApplication;
}

FDUIApplication::FDUIApplication() :
	ApplicationScale(1.f)
{

}


TSharedPtr<class DUIWindow> FDUIApplication::AddWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow)
{			
	TSharedPtr<FGenericWindow> NewWindow = MakeWindow(InWindow, bShow);
	Windows.Push(InWindow);

	if (bShow)
	{
		InWindow->ShowWindow();

	}

	
	return InWindow;

}

void FDUIApplication::InitHightDPI(bool bForceEnable)
{
	FPlatformApplicationMisc::SetHighDPIMode();



}

void FDUIApplication::Tick()
{
	PlatformApplication->PumpMessages(1.f);

	PlatformApplication->Tick(1.f);




}

DARKUI_API TSharedPtr<DUIWindow> FDUIApplication::GetActiveModalWindow() const
{
	return ActiveModalWindows.Num() > 0 ? ActiveModalWindows.Last() : nullptr;
}

bool FDUIApplication::ShouldProcessUserInputMessages(const TSharedPtr<FGenericWindow>& PlatformWindow) const
{
	TSharedPtr<DUIWindow> Window;

	if (PlatformWindow)
	{
		Window = FindWindowByPlatformWindow(Windows, PlatformWindow);
	}

	if (ActiveModalWindows.Num() == 0 || (Window && Window->IsSubWindowOf(GetActiveModalWindow())))
	{
		return true;
	}
	return false;
}

EWindowZone FDUIApplication::GetWindowZoneForPoint(const TSharedPtr<FGenericWindow>& Window, const int32 X, const int32 Y)
{
	TSharedPtr<DUIWindow> CurrentWindow = FindWindowByPlatformWindow(Windows, Window);
	if (CurrentWindow)
	{
		return CurrentWindow->GetWindowZoneForPoint(FVector2f(X, Y));
	}
	return EWindowZone::NotInWindow;

}

void FDUIApplication::OnWindowClose(const TSharedPtr<FGenericWindow>& Window)
{
	TSharedPtr<DUIWindow> CurrentWindow = FindWindowByPlatformWindow(Windows, Window);

	if (CurrentWindow)
	{
		bool bCanCloseWindow = true;
		//


		if (bCanCloseWindow)
		{
			Windows.Remove(CurrentWindow);
			Window->Destroy();
		}

	}
	

}

TSharedPtr<FGenericWindow> FDUIApplication::MakeWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow)
{
	TSharedPtr<FGenericWindow> NativeParent = nullptr;
	TSharedPtr<DUIWindow> ParentWindow = nullptr;

	FGenericWindowDefinition Definition;

	const FVector2f Size = InWindow->GetInitialDesiredSizeInScreen();
	const FVector2f Position = InWindow->GetInitialDesiredPositionInScreen();

	Definition.Type = InWindow->GetType();
	Definition.WidthDesiredOnScreen = Size.X;
	Definition.HeightDesiredOnScreen = Size.Y;

	Definition.XDesiredPositionOnScreen = Position.X;
	Definition.YDisiredPositionOnScreen = Position.Y;

	Definition.bHasOSWindowBorder = InWindow->HasOSBorder();
	Definition.bIsRegularWindow = InWindow->IsRegularWindow();

	//Definition.WidthDesiredOnScreen = 


	TSharedPtr<FGenericWindow> NewWindow = PlatformApplication->MakeWindow();

	InWindow->SetNativeWindow(NewWindow);
	InWindow->SetScreenPosition(Position);
	InWindow->SetSize(Size);

	PlatformApplication->InitializeWindow(NewWindow, Definition, NativeParent, bShow);

	return NewWindow;

}

void FDUIApplication::TickPlatform(float DeltaTime)
{
	PlatformApplication->PumpMessages(DeltaTime);



}

TSharedPtr<DUIWindow> FDUIApplication::FindWindowByPlatformWindow(const TArray<TSharedPtr<DUIWindow>>& WindowsToSearch, const TSharedPtr<FGenericWindow>& PlatformWindow)
{
	for (int32 i = 0; i < WindowsToSearch.Num(); ++i)
	{
		TSharedPtr<DUIWindow> Window = WindowsToSearch[i];
		TSharedPtr<FGenericWindow> NativeWindow = Window->GetNativeWindow();

		if (NativeWindow == PlatformWindow)
		{
			return Window;
		}

		TSharedPtr<DUIWindow> FoundChild = FindWindowByPlatformWindow(Window->GetChildWindows(), PlatformWindow);
		if (FoundChild)
			return FoundChild;
	}
	
	return nullptr;
}

float FDUIApplication::GetApplicationScale() const
{
	return ApplicationScale;
}

DARKUI_API void FDUIApplication::LocateWidgetInWindow(const FVector2d& MousePosition, const TSharedPtr<DUIWindow>& Window, bool bIgnoreEnabledStatus, int32 UserIndex) const
{
	const bool bAcceptInput = Window->IsVisible() && (Window->AcceptsInput() || IsWindowHousingInteractiveTooltip(Window));
	if (bAcceptInput && Window->IsScreenSpaceMouseWithin(MousePosition))
	{
		FVector2f CursorPosition = MousePosition;

		if (Window->GetWindowMode() == EWindowMode::FullScreen)
		{
			FVector2f WindowSize = Window->GetSize();
			FVector2f DisplaySize = { static_cast<float>(CachedDisplayMetrics.PrimaryDisplayWidth), static_cast<float>(CachedDisplayMetrics.PrimaryDisplayHeight) };
			
			CursorPosition *= WindowSize / DisplaySize;
		}

		
		//TArray<FWidgetAndPointer> WidgetsAndCursors 



	}



}

bool FDUIApplication::IsWindowHousingInteractiveTooltip(const TSharedPtr<const DUIWindow>& WindowToTest) const
{
	for (const auto& User : Users)
	{
		if (User && User->IsWindowHousingInteractiveTooltip(WindowToTest))
		{
			return true;
		}
	}
	return false;
}

void FDUIApplication::GetDisplayMetrics(FDisplayMetrics& OutMetrics) const
{
	PlatformApplication->RebuildDisplayMetrics(OutMetrics);
	CachedDisplayMetrics = OutMetrics;
}

void FDUIApplication::GetCachedDisplayMetrics(FDisplayMetrics& OutMetrics) const
{
	OutMetrics = CachedDisplayMetrics;
}
