#pragma once


class IRenderer
{
public:
	virtual void BeginRenderingView(class FCanvas* Canvas, class FSceneView* View) = 0;
	virtual void RenderUI(class FRenderTarget* RenderTarget) = 0;
};

class ISceneRenderer
{

};