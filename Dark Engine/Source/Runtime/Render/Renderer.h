#pragma once

#define D3D12
#define RENDER_API 


class Renderer
{
public:
	//Renderer();

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void BeginFrame() = 0;
	virtual void Render() = 0;
	virtual void EndFrame() = 0;


};

#ifdef _WIN32
#ifdef D3D12
#include "D3D12/D3D12Render.h"
#define RENDER_API D3D12Renderer
#endif
#ifdef GL
#include "OpenGL/OpenGLRender.h"
#define RENDER_API OpenGLRender
#endif
#endif // WIN32
