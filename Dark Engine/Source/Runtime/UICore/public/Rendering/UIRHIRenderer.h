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
	void Initialize();

	virtual void Destroy() {}

	virtual void CreateViewport(UIWindow* InWindow);
	virtual void DrawWindows() {}
	virtual void BeginFrame() {}
	virtual void EndFrame() {}


private:
	TMap<const UIWindow*, FViewportInfo*> WindowToViewportInfo;
	




};