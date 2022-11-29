#pragma once
#include "../Renderer.h"



//#include <gl/GL.h>

class OpenGLRender : public Renderer
{
	void Init() override {};
	void Shutdown() override {}

	void BeginFrame() override {}
	void Render() override {};
	void RenderObj() {};
	void RenderScene() {};
	void EndFrame() override {}


};