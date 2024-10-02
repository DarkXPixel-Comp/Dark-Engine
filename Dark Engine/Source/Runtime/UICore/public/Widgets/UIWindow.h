#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "Memory/TUniquePtr.h"
#include "CoreMinimal.h"
#include "UIWidget.h"
#include "DarkClient.h"
#include "imgui.h"
#include "Math/Vector2D.h"


class UIWindow
{
public:
	UIWindow() {}
	~UIWindow();

public:
	TSharedPtr<FGenericWindow> GetNativeWindow() const { return NativeWindow; }
	void SetNativeWindow(TSharedPtr<FGenericWindow> InNativeWindow);

	void ShowWindow();
	void Maximize() { NativeWindow->Maximize(); }
	void Minimize() { NativeWindow->Minimize(); }
	void MaximizeOrRestore() { IsMaximize() ? Restore() : Maximize(); }
	bool IsMaximize() const { return NativeWindow->IsMaximized(); }
	void Restore() const { NativeWindow->Restore(); }

	void Update(float DeltaTime);

	FVector2f GetInitSizeInScreen() const { return InitSizeScreen; }
	FVector2f GetInitPositionInScreen() const { return InitPositionScreen; }
	bool HasWindowBorder() const { return bHasWindowBorder; }
	void SetbWindowBorder(bool InbHasWindowBorder) { bHasWindowBorder = InbHasWindowBorder; }
	FString GetTitle() const { return Title; }
	void SetTitle(FString InTitle) { Title = InTitle; bTitleChanged = true; }
	void AddWidget(TSharedPtr<UIWidget> InWidget) { InWidget->InitWindow(this); Widgets.Add(InWidget); }
	void AddDeferedWidget(TSharedPtr<UIWidget>InWidget) { DeferedAddWidgets.Add(InWidget); }
	void RemoveDeferedWidget(TSharedPtr<UIWidget>InWidget) { DeferedRemoveWidgets.Add(InWidget); }
	void RemoveWidget(TSharedPtr<UIWidget> InWidget) { Widgets.RemovePtr(InWidget); }
	void RemoveWidget(UIWidget* InWidget) { Widgets.RemovePtr(InWidget); }
	//void SetViewportClient(TSharedPtr<FViewportClient> InViewportClient) { ViewportClient = InViewportClient; }
	void SetImGuiContext(ImGuiContext* InContext) { IMGUIContext = InContext; }
	ImGuiContext* GetImGuiContext() const { return IMGUIContext; }
	FVector2f GetViewportSize() { return SizeViewport; }
	const TArray<TSharedPtr<UIWidget>>& GetWidgets() const { return Widgets; }

	TSharedPtr<FViewportClient> CreateViewportClient();
	void CreateViewport();
	void DrawWindow();

	FIntPoint GetMousePosition() const;
	void SetSizeViewport(FVector2f InSize) { SizeViewport = InSize; }


private:
	bool bHasWindowBorder = true;
	TSharedPtr<FGenericWindow> NativeWindow;
	FVector2f InitSizeScreen = {1280, 720};
	FVector2f InitPositionScreen;
	FVector2f SizeViewport = InitSizeScreen;
	FString Title;
	bool bTitleChanged = true;
	//TSharedPtr<FViewportClient>
	TArray<TSharedPtr<UIWidget>> Widgets;
	TArray<TSharedPtr<UIWidget>> DeferedAddWidgets;
	TArray<TSharedPtr<UIWidget>> DeferedRemoveWidgets;
	//TSharedPtr<FUIWindowViewport> Viewport;
	TSharedPtr<FRHIViewport> Viewport;
	ImGuiContext* IMGUIContext;
};