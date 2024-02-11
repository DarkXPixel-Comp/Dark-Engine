#include "Application/UIApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Math/Vector2D.h"
#include "imgui.h"



TSharedPtr<UIApplication> UIApplication::CurrentApplication = nullptr;
TSharedPtr<FGenericApplication> UIApplication::PlatformApplication = nullptr;



TSharedPtr<UIApplication> UIApplication::Create(const TSharedPtr<FGenericApplication>& InPlatformApplictation)
{
	PlatformApplication = InPlatformApplictation;

	CurrentApplication = MakeShareble(new UIApplication());

	return CurrentApplication;
}

TSharedPtr<UIApplication> UIApplication::Create()
{
	return Create(MakeShareble(FPlatformApplicationMisc::CreateApplication()));
}


void UIApplication::InitializeRenderer(TSharedPtr<FUIRenderer> InRenderer)
{
	check(InRenderer);
	Renderer = InRenderer;
	Renderer->Initialize();
}

void UIApplication::Tick(float DeltaTime)
{
	PlatformApplication->PumpMessages();
	PlatformApplication->Tick(DeltaTime);
}


TSharedPtr<UIWindow> UIApplication::AddWindow(TSharedPtr<UIWindow> InWindow)
{
	TSharedPtr<FGenericWindow> NewWindow = MakeWindow(InWindow);

	return InWindow;

}

TSharedPtr<FGenericWindow> UIApplication::MakeWindow(TSharedPtr<UIWindow> InUIWindow)
{
	TSharedPtr<FGenericWindowDefinition> Definition = MakeShareble(new FGenericWindowDefinition());

	const FVector2f Size = InUIWindow->GetInitSizeInScreen();
	Definition->WidthOnScreen = Size.X;
	Definition->HeightOnScreen = Size.Y;

	const FVector2f Position = InUIWindow->GetInitPositionInScreen();
	Definition->XPositionOnScreen = Position.X;
	Definition->YPositionOnScreen = Position.Y;
	Definition->bHasWindowBorder = InUIWindow->HasWindowBorder();

	Definition->Title = InUIWindow->GetTitle();

	TSharedPtr<FGenericWindow> Window = PlatformApplication->MakeWindow();

	InUIWindow->SetNativeWindow(Window);
	
	PlatformApplication->InitializeWindow(Window, Definition);

	return Window;
}