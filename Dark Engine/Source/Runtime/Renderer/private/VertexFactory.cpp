#include "VertexFactory.h"


FVertexElement FVertexFactory::AccessStreamComponent(const FVertexStreamComponent& Component, uint8 AttributeIndex, EVertexInputStreamType InputStreamType)
{
	if (InputStreamType == EVertexInputStreamType::PositionOnly)
	{
		return AccessStreamComponent(Component, AttributeIndex, PositionStream);
	}
	if (InputStreamType == EVertexInputStreamType::PositionAndNormalOnly)
	{
		return AccessStreamComponent(Component, AttributeIndex, PositionAndNormalStream);
	}
	return AccessStreamComponent(Component, AttributeIndex, Streams);
}


void FVertexFactory::ReleaseRHI()
{
	Declaration.Reset();
	PositionDeclaration.Reset();
	PositionAndNormalDeclaration.Reset();
	PositionAndNormalStream.Empty();
	Streams.Empty();
	PositionStream.Empty();
}