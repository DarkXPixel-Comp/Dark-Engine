#pragma once


class IRenderer
{
public:
	virtual void BeginRenderingView(class FCanvas* Canvas, class FSceneView* View) = 0;
};

class ISceneRenderer
{

};