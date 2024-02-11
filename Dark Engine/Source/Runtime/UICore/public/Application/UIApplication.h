#pragma once
#include "CoreMinimal.h"
#include "Widgets/UIWindow.h"
#include "Widgets/UIViewport.h"
#include "GenericPlatform/GenericApplication.h"
#include "Rendering/UIRenderer.h"




//STATIC CLASS
class UIApplication
{
public:
	static UIApplication* Get() { return CurrentApplication.get(); }


public:
	static TSharedPtr<UIApplication> Create(const TSharedPtr<FGenericApplication>& InPlatformApplictation);
	static TSharedPtr<UIApplication> Create();

public:
	void InitializeRenderer(TSharedPtr<FUIRenderer> InRenderer);
	TSharedPtr<UIWindow> AddWindow(TSharedPtr<UIWindow> InWindow);
	void RegisterGameViewport(TSharedPtr<UIViewport> InViewport);


private:
	TSharedPtr<FGenericWindow> MakeWindow(TSharedPtr<UIWindow> InUIWindow);



private:
	static TSharedPtr<UIApplication> CurrentApplication;
	static TSharedPtr<FGenericApplication>	PlatformApplication;
private:
	TArray<TSharedPtr<UIWindow>> UIWindows;

	TWeakPtr<UIViewport> GameViewport;

	TSet<TWeakPtr<UIViewport>> AllGameViewports;

	TSharedPtr<FUIRenderer>	Renderer;
};