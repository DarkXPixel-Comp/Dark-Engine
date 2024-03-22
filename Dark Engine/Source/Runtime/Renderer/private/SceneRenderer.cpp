#include "SceneRenderer.h"
#include "SceneView.h"
#include "Canvas.h"
#include "Engine/World.h"
#include "ScenePrivate.h"
#include "SceneRendering.h"
#include <RHICommandList.h>



static void RenderView(FRHICommandListImmediate& RHICmdList, FSceneRender* SceneRenderer)
{
	FScene* const Scene = SceneRenderer->Scene;


	SceneRenderer->Render(RHICmdList);
	SceneRenderer->Flush();



}




void FRenderer::BeginRenderingView(FCanvas* Canvas,FSceneView* View)
{
	check(Canvas);
	check(View);
	check(View->Scene);

	FWorld* World = nullptr;

	FScene* const Scene = View->Scene->GetRenderScene();
	if (Scene)
	{
		World = Scene->GetWorld();
		if (World)
		{

		}
	}


	Canvas->Flush();

	Scene->IncrementFrameNumber();
	//View->FrameNumber = Scene->GetFrameNumber();
	FSceneRender* SceneRender = new FSceneRender(View);

	FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();


	//SceneRender->UpdateReflection(;)

	RenderView(RHICmdList, SceneRender);




	//ImGui::DRAW

	delete SceneRender;
}

void FRenderer::RenderUI(FRenderTarget* RenderTarget)
{

}
