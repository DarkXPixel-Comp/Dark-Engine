#include "DarkClient.h"


const TRefCountPtr<FRHITexture>& FRenderTarget::GetRenderTargetTexture() const
{
	return RenderTargetTexture;
	
}






void FViewport::Draw(FViewportClient* InViewport)
{
	ViewportClient = InViewport;
	FWorld* World = nullptr;//GetClient()->GetWorld();


	ViewportClient->Draw(this, World);













}