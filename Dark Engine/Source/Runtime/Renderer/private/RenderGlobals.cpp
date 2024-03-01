#include "RenderGlobals.h"
#include "Memory/TUniquePtr.h"
#include "SceneRenderer.h"




TSharedPtr<IRenderer> Renderer = nullptr;


IRenderer* GetRenderer()
{
	if (!Renderer)
	{
		Renderer = MakeShareble(new FRenderer());
	}
	return Renderer.get();
}

