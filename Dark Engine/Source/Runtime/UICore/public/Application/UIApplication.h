#pragma once
#include "CoreMinimal.h"
#include "Widgets/UIWindow.h"
#include "Widgets/UIViewport.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericApplicationHandler.h"
#include "Rendering/UIRenderer.h"

#include "Delegate/Delegate.h"



DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnMouseDown, UIWindow*,
	const EMouseButtons::Type, const FVector2D);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMouseUp, const EMouseButtons::Type, const FVector2D);







//STATIC CLASS
class UIApplication	: public FGenericApplicationHandler
{
public:
	static UIApplication* Get() { return CurrentApplication.get(); }


public:
	static TSharedPtr<UIApplication> Create(const TSharedPtr<FGenericApplication>& InPlatformApplictation);
	static TSharedPtr<UIApplication> Create();


public:
	virtual bool OnSizeChanged(const TSharedPtr<FGenericWindow>& Window, const int32 Width, const int32 Height, bool bWasMinimized = false);
	virtual bool OnMouseDown(const TSharedPtr<FGenericWindow>& Window, const EMouseButtons::Type Button,
		const FVector2D CursorPos);
	virtual bool OnMouseUp(const EMouseButtons::Type Button,
		const FVector2D CursorPos) override;

public:
	void InitializeRenderer(TSharedPtr<FUIRenderer> InRenderer);
	TSharedPtr<UIWindow> AddWindow(TSharedPtr<UIWindow> InWindow);
	void RegisterGameViewport(TSharedPtr<UIViewport> InViewport);
	void Tick();
	void TickPlatform(float DeltaTime);
	void TickAndDrawWidgets(float DeltaTime);
	void DrawWindows();

	TSharedPtr<FUIRenderer>	GetRenderer() const { return Renderer; }

	FIntPoint GetMousePosition() const;

private:
	TSharedPtr<FGenericWindow> MakeWindow(TSharedPtr<UIWindow> InUIWindow);

	UIWindow* const FindUIWindowByNative(TSharedPtr<FGenericWindow> InWindow);




private:
	static TSharedPtr<UIApplication> CurrentApplication;
	static TSharedPtr<FGenericApplication>	PlatformApplication;

public:
	FOnMouseDown OnMouseDownDelegate;
	FOnMouseUp OnMouseUpDelegate;

private:
	TArray<TSharedPtr<UIWindow>> UIWindows;

	TWeakPtr<UIViewport> GameViewport;

	TArray<TWeakPtr<UIViewport>> AllGameViewports;

	TSharedPtr<FUIRenderer>	Renderer;

	TArray<UIWidget> Widgets;
};