#pragma once
#include "RendererInterface.h"




class FRenderer	: public IRenderer
{
public:
	FRenderer() {}
	void BeginRenderingView(class FCanvas* Canvas, class FSceneView* View);

};

