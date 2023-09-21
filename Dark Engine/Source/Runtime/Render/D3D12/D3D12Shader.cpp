#include "D3D12Shader.h"
#include <Logging/Logger.hpp>
#include <CoreDefines.h>

D3D12Shader::D3D12Shader(eD3D12ShaderType type, FString path)
{
	m_shaderType = type;
	m_shaderPath = path;

	if (path == "")
		return;


#ifndef _DEVELOPMENT
	Logger::log("It is recommended to compile the shader manually", LOGGER_WARNING);
#endif // _DEVELOPMENT
	CompileShader(type, path);

}

D3D12Shader::D3D12Shader(eD3D12ShaderType type, ID3DBlob* shaderBlob)
{
	m_shaderType = type;
	LoadShader(type, shaderBlob);
}

bool D3D12Shader::CompileShader(eD3D12ShaderType type, FString path)
{
	ComPtr<ID3DBlob> ErrorBlob;

	switch (type)
	{
	case D3D12_SHADER_TYPE_VERTEX:
		DXCall(D3DCompileFromFile(strw(path).c_str(), nullptr, nullptr,
			"main", "vs_5_1", 0, 0, &m_shaderBlob, &ErrorBlob));
		break;
	case D3D12_SHADER_TYPE_PIXEL:
		DXCall(D3DCompileFromFile(strw(path).c_str(), nullptr, nullptr,
			"main", "ps_5_1", 0, 0, &m_shaderBlob, &ErrorBlob));
		break;
	case D3D12_SHADER_TYPE_GEOMETRY:
	case D3D12_SHADER_TYPE_DOMEN:
	case D3D12_SHADER_TYPE_TESSELLATION:
	case D3D12_SHADER_TYPE_RAY_TRACING:
	default:
		memset(m_shaderBlob.Get(), 0, sizeof(ID3DBlob));
		return false;
	}


	if (ErrorBlob)
	{
		FString error = static_cast<const char*>(ErrorBlob->GetBufferPointer());
		Logger::log(error, LOGGER_ERROR);
		memset(m_shaderBlob.Get(), 0, sizeof(ID3DBlob));
		return false;
	}


	return true;
}

bool D3D12Shader::LoadShader(eD3D12ShaderType type, ID3DBlob* shaderBlob)
{
	if (!shaderBlob)
		return false;
	m_shaderBlob = shaderBlob;
	m_shaderType = type;
	m_shaderName = "LOAD SHADER";
	m_shaderPath = "LOAD SHADER";


	return false;
}
