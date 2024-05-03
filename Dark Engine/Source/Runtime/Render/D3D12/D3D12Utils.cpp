#include "D3D12Utils.h"
#include "D3D12Render.h"
#include "D3D12PSO.h"
#include "D3D12Texture.h"
#include <DEngine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <DDSTextureLoader.h>
#include <ResourceUploadBatch.h>
#include "Misc/Paths.h"
#include <fstream>


std::unordered_map<UINT, TUniquePtr<D3D12PipelineShaderRootSignature>> D3DUtil::Pipelines;
std::unordered_map<std::string, TUniquePtr<FD3D12Mesh>> D3DUtil::m_meshes;
std::unordered_map<std::string, TUniquePtr<D3D12Texture>> D3DUtil::m_textures;
TArray<CD3DX12_STATIC_SAMPLER_DESC> D3DUtil::m_samplers(6);
ComPtr<IDxcCompiler3> D3DUtil::m_ShaderCompiler;
ComPtr<IDxcUtils>  D3DUtil::m_Utils;

