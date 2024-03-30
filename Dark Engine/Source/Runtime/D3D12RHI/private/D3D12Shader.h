#pragma once
#include "RHIResources.h"

struct FD3D12ShaderData
{
	TArray<uint8> Code;



};


class FD3D12VertexShader : public FRHIVertexShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Vertex };

};


class FD3D12PixelShader : public FRHIPixelShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Pixel };

};

class FD3D12GeometryShader : public FRHIGeometryShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Geometry };

};

class FD3D12ComputeShader : public FRHIComputeShader, public FD3D12ShaderData
{
public:
	enum { StaticType = ST_Compute };

};