#include "PrimitiveSceneProxy.h"
#include "PrimitiveSceneInfo.h"
#include "SceneInterface.h"


FPrimitiveSceneProxy::FPrimitiveSceneProxy(const GPrimitiveComponent* InComponent, const FString& Name)	:
	FPrimitiveSceneProxy(FPrimitiveSceneProxyDesc(InComponent), Name)
{


}

FPrimitiveSceneProxy::FPrimitiveSceneProxy(const FPrimitiveSceneProxyDesc& InDesc, const FString& Name)
{



}

void FPrimitiveSceneProxy::SetTransform(const FMatrix& InMatrix)
{

}
