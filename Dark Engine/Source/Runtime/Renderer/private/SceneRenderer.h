#pragma once
#include "RendererInterface.h"
#include "Containers/Array.h"




class FRenderer	: public IRenderer
{
public:
	FRenderer() {}
	void BeginRenderingView(class FCanvas* Canvas, class FSceneView* View);
	virtual class FSceneInterface* AllocateScene(class FWorld* World);
	virtual void RemoveScene(class FSceneInterface* Scene);

private:
	TArray<class FSceneInterface*> AllocatedScenes;

};

