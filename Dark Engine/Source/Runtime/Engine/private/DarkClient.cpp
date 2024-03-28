#include "DarkClient.h"
#include "Canvas.h"


const TRefCountPtr<FRHITexture>& FRenderTarget::GetRenderTargetTexture() const
{
	return RenderTargetTextureRHI;
	
}


FViewport::FViewport(FViewportClient* InViewportClient):
	ViewportClient(InViewportClient),
	SizeX(0),
	SizeY(0)
{

}

void FViewport::Draw()
{
	FCanvas Canvas(this, GetClient()->GetWorld());
	


	ViewportClient->Draw(this, &Canvas);

}