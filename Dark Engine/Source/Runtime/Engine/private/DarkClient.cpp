#include "DarkClient.h"


const TRefCountPtr<FRHITexture>& FRenderTarget::GetRenderTargetTexture() const
{
	return RenderTargetTextureRHI;
	
}






void FViewport::Draw(FViewportClient* InViewport)
{
	ViewportClient = InViewport;
	FWorld* World = GetClient()->GetWorld();

	ViewportClient->Draw(this, World);

}