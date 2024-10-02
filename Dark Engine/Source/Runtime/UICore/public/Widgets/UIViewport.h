#pragma once
#include "CoreMinimal.h"
#include "Rendering/RenderingCommon.h"
#include "UIWidget.h"
#include "Engine/EditorViewportClient.h"
#include "Math/Vector2D.h"
//#include "Engine/SceneViewport.h"




class FSceneViewport;

class UIViewport : public virtual UIWidget
{
public:
	UIViewport();
	virtual ~UIViewport() = default;
//	void SetViewport(TSharedPtr<IUIViewport> Viewport) { ViewportInterface = Viewport; }

	virtual void Update(float DeltaTime);

	virtual void DrawImGui();



protected:
	TSharedPtr<FSceneViewport> Viewport;
	TSharedPtr<FViewportClient>	ViewportClient;
	FVector2D ViewportSize;
	float Delta = 0.f;

};