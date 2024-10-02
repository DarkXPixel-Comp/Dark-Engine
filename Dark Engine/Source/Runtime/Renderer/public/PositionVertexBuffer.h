#pragma once
#include "RenderResource.h"
#include "Math/Color.h"
#include "Math/Vector2D.h"


struct FStaticMeshBuildVertex
{
	FVector3f Position;

	FVector3f TangentX;
	FVector3f TangentY;
	FVector3f TangentZ;

	FVector2f UV[8];
	FColor Color;
};




struct FPositionVertex
{
	FVector3f Position;
};



class FPositionVertexBuffer : public FVertexBuffer
{
public:
	void Init(uint32 NumVertices, bool bNeedCPUAccess = true);
	void Init(const TArray<FStaticMeshBuildVertex>& InVertices, bool bNeedCPUAccess = true);

	void Init(const TArray<FVector3f>& InPositions, bool bNeedCPUAccess = true);

	virtual void InitRHI(FRHICommandListImmediate& RHICmdList) override;

	FVector3f& VertexPosition(uint32 Index)
	{
		return VertexData[Index].Position;
	}


private:
	TArray<FPositionVertex> VertexData;
	uint8* Data;
	uint32 Stride = sizeof(FPositionVertex);
	uint32 NumVertices;
	bool bNeedCPUAccess = true;

	


};