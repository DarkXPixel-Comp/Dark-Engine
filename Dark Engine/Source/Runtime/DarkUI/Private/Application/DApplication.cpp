#include "Application/DApplication.h"
#include "Widgets/DWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Platform/PlatformApplicationMisc.h"


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

FDUIApplication::FDUIApplication()
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
