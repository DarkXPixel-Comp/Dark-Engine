#pragma once
#include "Containers/DarkString.h"
#include "Math/Matrix.h"



struct FPrimitiveSceneProxyDesc
{
	FPrimitiveSceneProxyDesc()
	{

	}

	FPrimitiveSceneProxyDesc(const class GPrimitiveComponent* InComponent) {}
};

struct FPrimitiveDescription
{
	class GPrimitiveComponent* Component = nullptr;
	FMatrix ModelMatrix = FMatrix::Identity;

	bool IsValid() const
	{
		Component != nullptr;
	}


};


class FPrimitiveSceneProxy
{
public:
	FPrimitiveSceneProxy(const class GPrimitiveComponent* InComponent, const FString& Name);
	FPrimitiveSceneProxy(const FPrimitiveSceneProxyDesc& InDesc, const FString& Name);


	//void SetTransform(const FMatrix& InLocalToWorld, FVector InPosition);

	void SetTransform(const FMatrix& InMatrix);


	class FPrimitiveSceneInfo* PrimitiveSceneInfo = nullptr;
	class GStaticMesh* StaticMesh = nullptr; //<- Temp


	class FSceneInterface* Scene;
private:
};


