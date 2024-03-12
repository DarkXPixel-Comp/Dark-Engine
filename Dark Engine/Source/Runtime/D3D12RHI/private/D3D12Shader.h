#pragma once
#include "RHIResources.h"

struct FD3D12ShaderData
{

};


class FD3D12VertexShader : public FRHIVertexShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Vertex };

};