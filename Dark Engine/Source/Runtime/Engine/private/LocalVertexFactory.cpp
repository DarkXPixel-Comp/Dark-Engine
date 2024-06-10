#include "LocalVertexFactory.h"

void FLocalVertexFactory::SetData(FRHICommandListImmediate& RHICmdList, const FStaticMeshDataType& InData)
{
	Data = InData;

	UpdateRHI(RHICmdList);

}

void FLocalVertexFactory::InitRHI(FRHICommandListImmediate& RHICmdList)
{
	if (Data.PositionComponent.VertexBuffer != Data.TangentBasisComponent[0].VertexBuffer)
	{
		auto AddDeclaration = [this](EVertexInputStreamType InputStreamType, bool bAddNormal)
			{
				FVertexDeclarationElementList StreamElements;
				StreamElements.Add(AccessStreamComponent(Data.PositionComponent, 0, InputStreamType));

				bAddNormal = bAddNormal && Data.TangentBasisComponent[1].VertexBuffer != NULL;
				if (bAddNormal)
				{
					StreamElements.Add(AccessStreamComponent(Data.TangentBasisComponent[1], 2, InputStreamType));
				}

				PrimitiveIdStreamIndex[static_cast<int32>(InputStreamType)] = static_cast<int8>(StreamElements.Last().StreamIndex);

				InitDeclaration(StreamElements, InputStreamType);;

			};

		AddDeclaration(EVertexInputStreamType::PositionOnly, false);
		AddDeclaration(EVertexInputStreamType::PositionAndNormalOnly, true);
	}

	uint32 Null;
	FVertexDeclarationElementList Elements;
	GetVertexElements(EVertexInputStreamType::Default, false, Data, Elements, Streams, Null);

	PrimitiveIdStreamIndex[static_cast<int32>(EVertexInputStreamType::Default)] = static_cast<int8>(Elements.Last().StreamIndex);

	InitDeclaration(Elements, EVertexInputStreamType::Default);
}

void FLocalVertexFactory::GetVertexElements(EVertexInputStreamType StreamType, bool bSupportManualVertexFetch, FStaticMeshDataType& Data, FVertexDeclarationElementList& Elements, FVertexStreamList& InOutStreams, uint32& OutColorStreamIndex)
{
	check(StreamType == EVertexInputStreamType::Default);



	if (Data.PositionComponent.VertexBuffer != nullptr)
	{
		Elements.Add(AccessStreamComponent(Data.PositionComponent, 0, InOutStreams));
	}

	if (!bSupportManualVertexFetch)
	{
		uint8 TangentBasisAttributes[2] = { 1, 2 };

		for (uint32 AxisIndex = 0; AxisIndex < 2; ++AxisIndex)
		{
			if (Data.TangentBasisComponent[AxisIndex].VertexBuffer != nullptr)
			{
				Elements.Add(AccessStreamComponent(Data.TangentBasisComponent[AxisIndex], TangentBasisAttributes[AxisIndex], InOutStreams));
			}

			if (Data.ColorComponent.VertexBuffer)
			{
				Elements.Add(AccessStreamComponent(Data.ColorComponent, 3, InOutStreams));
			}
			else
			{
				//FVertexStreamComponent NullColorComponent()
			}

			if (Data.TextureCoordinates.Num())
			{
				const uint32 BaseTexCoordAttribute = 4;
				for (uint32 CoordinateIndex = 0; CoordinateIndex < Data.TextureCoordinates.Num(); ++CoordinateIndex)
				{
					Elements.Add(AccessStreamComponent(Data.TextureCoordinates[CoordinateIndex], BaseTexCoordAttribute + CoordinateIndex, InOutStreams));
				}

				for (uint32 CoordinateIndex = Data.TextureCoordinates.Num(); CoordinateIndex < 4; ++CoordinateIndex)
				{
					Elements.Add(AccessStreamComponent(Data.TextureCoordinates[Data.TextureCoordinates.Num()], BaseTexCoordAttribute + CoordinateIndex, InOutStreams));
				}
			}

			if (Data.LightMapCoordinateComponent.VertexBuffer)
			{
				Elements.Add(AccessStreamComponent(Data.LightMapCoordinateComponent, 15, InOutStreams));
			}
			else if (Data.TextureCoordinates.Num())
			{
				Elements.Add(AccessStreamComponent(Data.TextureCoordinates[0], 15, InOutStreams));
			}
		}
	}
}


void FLocalVertexFactory::InitDeclaration(const FVertexDeclarationElementList& Elements, EVertexInputStreamType StreamType)
{
	if (StreamType == EVertexInputStreamType::PositionOnly)
	{
		PositionDeclaration = RHICreateVertexDeclaration(Elements);
	}
	if (StreamType == EVertexInputStreamType::PositionAndNormalOnly)
	{
		PositionAndNormalDeclaration = RHICreateVertexDeclaration(Elements);
	}																		
	if (StreamType == EVertexInputStreamType::Default)
	{
		Declaration = RHICreateVertexDeclaration(Elements);
	}
}