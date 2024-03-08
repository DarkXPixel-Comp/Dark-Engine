#pragma once

#include "D3D12Main.h"


class D3D12Texture
{
public:
	FString name;
	ID3D12Resource* m_textureBuffer;

	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc;
	int64_t m_descriptorIndex = -1;
};