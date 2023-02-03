#pragma once
#include "D3D12Utils.h"
#include "D3D12.h"
#include <vector>

template <typename T>
class D3D12UploadBufferResource
{
	std::vector<T> m_dataBuffer;
	UINT m_elementByteSize;
	UINT m_elementsByteSize;
	ID3D12Device8* m_device;
	ComPtr<ID3D12Resource> m_uploadBuffer;
	BYTE* m_mappedData;


public:
	D3D12UploadBufferResource(UINT elementCount, bool isConstantBuffer = false) ;
	~D3D12UploadBufferResource();

	void CopyData(UINT elementIndex, const T& data);
	T ReadData(UINT elementIndex) { return m_dataBuffer[elementIndex]; }
	ID3D12Resource* GetResource() { return m_uploadBuffer.Get(); }
};

