#include "Application/DApplication.h"
#include "Widgets/DWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Platform/PlatformApplicationMisc.h"
#include "Application/DUser.h"


TSharedPtr<FDUIApplication> FDUIApplication::CurrentApplication;
TSharedPtr<FGenericApplication> FDUIApplication::PlatformApplication;

void FDUIApplication::Create()
{
	Create(MakeShareble(FPlatformApplicationMisc::CreateApplication()));
}

TSharedPtr<FDUIApplication> FDUIApplication::Create(const TSharedPtr<FGenericApplication>& InPlatformApplication)  
{  
	PlatformApplication = InPlatformApplication;

	CurrentApplication = MakeShareble(new FDUIApplication());

	PlatformApplication->SetMessageHandler(CurrentApplication);

	return CurrentApplication;
}

FDUIApplication::FDUIApplication() :
	ApplicationScale(1.f)
{

}


TSharedPtr<class DUIWindow> FDUIApplication::AddWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow)
{
	TSharedPtr<FGenericWindow> NewWindow = MakeWindow(InWindow, bShow);

	if (bShow)
	{
		//InWindow->Show();

	}

	
	return InWindow;

}

void FDUIApplication::InitHightDPI(bool bForceEnable)
{
	FPlatformApplicationMisc::SetHighDPIMode();



}

void FDUIApplication::Tick()
{



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
		//return CurrentWindow->GetCurr
	}
	return EWindowZone::NotInWindow;

}

TSharedPtr<FGenericWindow> FDUIApplication::MakeWindow(const TSharedPtr<class DUIWindow>& InWindow, bool bShow)
{
	TSharedPtr<FGenericWindow> NativeParent = nullptr;
	TSharedPtr<DUIWindow> ParentWindow = nullptr;

	FGenericWindowDefinition Definition;

	Definition.Type = InWindow->GetType();


	TSharedPtr<FGenericWindow> NewWindow = PlatformApplication->MakeWindow();

	InWindow->SetNativeWindow(NewWindow);
	InWindow->SetScreenPosition(InWindow->GetInitialDesiredPositionInScreen());
	InWindow->SetSize(InWindow->GetInitialDesiredSizeInScreen());

	PlatformApplication->InitializeWindow(NewWindow, Definition, NativeParent, bShow);

	return NewWindow;

}

void FDUIApplication::TickPlatform(float DeltaTime)
{
	PlatformApplication->PumpMessages();



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

		return FindWindowByPlatformWindow(Window->GetChildWindows(), PlatformWindow);
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
		if (Window->GetWindowMode() == EWindowMode::FullScreen)
		{
			FVector2f WindowSize = Window->GetSize();
			//FVector2f DisplaySize = 
		}


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
