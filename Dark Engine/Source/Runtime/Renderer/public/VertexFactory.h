#pragma once


#include "RenderResource.h"
#include "RHIResources.h"
#include "StaticMeshResources.h"


enum class EVertexInputStreamType : uint8
{
	Default = 0,
	PositionOnly,
	PositionAndNormalOnly,
	Count
};




class FVertexFactory : public FRenderResource
{
public:
	void ReleaseRHI();



	struct FVertexStream
	{
		const FVertexBuffer* VertexBuffer = nullptr;
		uint32 Offset = 0;
		uint16 Stride = 0;
		uint8 Padding = 0;
		EVertexStreamUsage VertexStreamUsage = EVertexStreamUsage::Default;
	};

	typedef TArray<FVertexStream> FVertexStreamList;

	FVertexElement AccessStreamComponent(const FVertexStreamComponent& Component, uint8 AttributeIndex, EVertexInputStreamType InputStreamType);


	template <typename VertexStreamListType>
	static FVertexElement AccessStreamComponent(const FVertexStreamComponent& Component, uint8 AttributeIndex, VertexStreamListType& InOutStreams)
	{
		FVertexStream VertexStream;
		VertexStream.VertexBuffer = Component.VertexBuffer;
		VertexStream.Stride = Component.Stride;
		VertexStream.Offset = Component.StreamOffset;
		VertexStream.VertexStreamUsage = Component.StreamUsage;
		TStaticArray<FVertexStream, 2> T;

		return FVertexElement((uint8)InOutStreams.Add(VertexStream), Component.Offset, Component.Type, AttributeIndex, VertexStream.Stride, EnumHasAnyFlags(EVertexStreamUsage::Instancing, VertexStream.VertexStreamUsage));
	}


	TArray<FVertexStream> Streams = TArray<FVertexStream>(ArrayReserve(8));
protected:
	TArray<FVertexStream> PositionStream = TArray<FVertexStream>(ArrayReserve(2));
	TArray<FVertexStream> PositionAndNormalStream = TArray<FVertexStream>(ArrayReserve(3));


	TRefCountPtr<FRHIVertexDeclaration>	Declaration;
	TRefCountPtr<FRHIVertexDeclaration>	PositionDeclaration;
	TRefCountPtr<FRHIVertexDeclaration>	PositionAndNormalDeclaration;
	

	int8 PrimitiveIdStreamIndex[(uint32)EVertexInputStreamType::Count];
};