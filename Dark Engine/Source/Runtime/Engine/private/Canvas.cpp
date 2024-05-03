#include "Canvas.h"
#include "SceneInterface.h"




FCanvas::FCanvas(FRenderTarget* InRenderTarget, FWorld* InWorld):
	ViewRect(0, 0, 0, 0),
	ScissorRect(0, 0, 0, 0),
	RenderTarget(InRenderTarget),
	Scene(InWorld ? InWorld->Scene : nullptr)
{


}

void FCanvas::Flush()
{
	check(RenderTarget);
}
