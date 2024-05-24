#include "SceneRenderer.h"
#include "SceneView.h"
#include "Canvas.h"
#include "Engine/World.h"
#include "ScenePrivate.h"
#include "SceneRendering.h"
#include <RHICommandList.h>




static void RenderView(FRHICommandListImmediate& RHICmdList, FSceneRender* SceneRenderer)
{
	//FScene* const Scene = SceneRenderer->Scene;


	//RHICmdList.BeginRenderPass(RenderPassInfo);
	SceneRenderer->Render(RHICmdList);
	//RHICmdList.EndRenderPass(RenderPassInfo);
	SceneRenderer->Flush();



}




void FRenderer::BeginRenderingView(FCanvas* Canvas, FSceneView* View)
{
	check(Canvas);
	check(View);
	//check(View->Scene);

	FWorld* World = nullptr;

	FScene* const Scene = View->Scene ? View->Scene->GetRenderScene() : nullptr;
	if (Scene)
	{
		World = Scene->GetWorld();
		if (World)
		{

		}
	}


	Canvas->Flush();
	FSceneRender* SceneRender;

	if (Scene)
	{
		Scene->IncrementFrameNumber();
	}

	SceneRender = new FSceneRender(View);
	FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();


	//SceneRender->UpdateReflection(;)

	RenderView(RHICmdList, SceneRender);
	//if (Scene)
	//{
	//	RenderView(RHICmdList, SceneRender);
	//}




	//ImGui::DRAW

	delete SceneRender;
}

FSceneInterface* FRenderer::AllocateScene(FWorld* World)
{
	FScene* NewScene = new FScene(World);
	AllocatedScenes.Add(NewScene);
	return NewScene;
}

void FRenderer::RemoveScene(FSceneInterface* Scene)
{
	check(Scene);
	delete Scene;
	AllocatedScenes.Remove(Scene);
}
