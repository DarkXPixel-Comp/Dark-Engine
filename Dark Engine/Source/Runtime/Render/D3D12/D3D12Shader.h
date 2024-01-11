#pragma once
#include <Core.h>
#include <Containers/DarkString.h>
#include "D3D12Main.h"


enum eD3D12ShaderType
{
	D3D12_SHADER_TYPE_VERTEX,
	D3D12_SHADER_TYPE_PIXEL,
	D3D12_SHADER_TYPE_GEOMETRY,
	D3D12_SHADER_TYPE_DOMEN,
	D3D12_SHADER_TYPE_TESSELLATION,
	D3D12_SHADER_TYPE_RAY_TRACING
};


class DENGINE_API D3D12Shader 
{
public:
	D3D12Shader();
	D3D12Shader(eD3D12ShaderType type, FString path);
	D3D12Shader(eD3D12ShaderType type, ID3DBlob* shaderBlob);

	bool CompileShader(eD3D12ShaderType type, FString path);
	bool LoadShader(eD3D12ShaderType type, ID3DBlob* shaderBlob);

private:
	ComPtr<ID3DBlob> m_shaderBlob;
	FString m_shaderPath;
	FString m_shaderName;
	eD3D12ShaderType m_shaderType;







};