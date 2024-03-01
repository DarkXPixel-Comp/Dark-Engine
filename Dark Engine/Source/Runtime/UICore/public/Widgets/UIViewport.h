#pragma once
#include "CoreMinimal.h"
#include "Rendering/RenderingCommon.h"
#include "UIWidget.h"
#include "Math/MathFwd.h"
#include "Engine/EditorViewportClient.h"
//#include "Engine/SceneViewport.h"


class FSceneViewport;

class UIViewport : public UIWidget
{
public:
	UIViewport();
//	void SetViewport(TSharedPtr<IUIViewport> Viewport) { ViewportInterface = Viewport; }





protected:
	TSharedPtr<FSceneViewport> Viewport;
	TSharedPtr<FViewportClient>	ViewportClient;
	FVector2D ViewportSize;







};