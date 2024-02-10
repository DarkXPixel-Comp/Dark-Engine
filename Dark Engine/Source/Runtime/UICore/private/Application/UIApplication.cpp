#include "Application/UIApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "imgui.h"



TSharedPtr<UIApplication> UIApplication::CurrentApplication = nullptr;
TSharedPtr<FGenericApplication> UIApplication::CurrentApplication = nullptr;



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