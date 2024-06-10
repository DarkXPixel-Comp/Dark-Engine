#pragma once
#include "VertexFactory.h"
#include "StaticMeshResources.h"



class FLocalVertexFactory : public FVertexFactory
{
public:
	FLocalVertexFactory(const TCHAR* InDebugName) : DebugName(InDebugName)
	{

	}

	void SetData(FRHICommandListImmediate& RHICmdList, const FStaticMeshDataType& InData);


	void InitDeclaration(const FVertexDeclarationElementList& Elements, EVertexInputStreamType StreamType);

	virtual void InitRHI(FRHICommandListImmediate& RHICmdList) override;
	virtual void ReleaseRHI() override
	{
		FVertexFactory::ReleaseRHI();
	}

	void GetVertexElements(EVertexInputStreamType StreamType,
		bool bSupportManualVertexFetch,
		FStaticMeshDataType& Data,
		FVertexDeclarationElementList& Elements,
		FVertexStreamList& InOutStreams,
		uint32& OutColorStreamIndex);


private:
	const TCHAR* DebugName;
	FStaticMeshDataType Data;


};