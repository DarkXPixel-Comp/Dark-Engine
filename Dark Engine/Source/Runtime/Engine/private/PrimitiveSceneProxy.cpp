#include "PrimitiveSceneProxy.h"


FPrimitiveSceneProxy::FPrimitiveSceneProxy(const GPrimitiveComponent* InComponent, const FString& Name)	:
	FPrimitiveSceneProxy(FPrimitiveSceneProxyDesc(InComponent), Name)
{


}

FPrimitiveSceneProxy::FPrimitiveSceneProxy(const FPrimitiveSceneProxyDesc& InDesc, const FString& Name)
{



}
