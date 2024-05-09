#pragma once
#include "Rendering/UIRenderer.h"
#include "Widgets/UIWindow.h"
#include "Containers/UnordoredMap.h"



struct FViewportInfo
{
	void* WindowHandle;
	int32 Width;
	int32 Height;
	TSharedPtr<FRHIViewport> ViewportRHI;
	bool bFullscreen;




};

class FUIRHIRenderer : public FUIRenderer
{
public:
	virtual ~FUIRHIRenderer() {}

	void Initialize();

	virtual void Destroy() {}

	virtual void CreateViewport(UIWindow* InWindow);
	virtual void DrawWindows() {}
	virtual void BeginFrame() {}
	virtual void EndFrame() {}
	virtual void Resize(UIWindow* InWindow, const int32 Width, const int32 Height, const bool bWasMinimized);

	virtual void DrawWindows(const TArray<TSharedPtr<UIWindow>>& Windows);


private:
	TMap<const UIWindow*, FViewportInfo*> WindowToViewportInfo;
	




};