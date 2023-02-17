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
	UINT GetElementByteSize() { return m_elementByteSize; }
	UINT GetElementsByteSize() { return m_elementsByteSize; }
};


template<typename T>
D3D12UploadBufferResource<T>::D3D12UploadBufferResource(UINT elementCount, bool isConstantBuffer)
{
	m_device = D3DUtil::GetDevice();
	m_dataBuffer.resize(elementCount);
	m_elementByteSize = isConstantBuffer ? D3DUtil::CalcConstantBufferByteSize(sizeof(T)) : sizeof(T);
	m_elementsByteSize = elementCount * m_elementByteSize;

	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(m_elementsByteSize);

	m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_uploadBuffer));
	m_uploadBuffer->Map(0, 0, reinterpret_cast<void**>(&m_mappedData));
}

template<typename T>
inline D3D12UploadBufferResource<T>::~D3D12UploadBufferResource()
{
	m_uploadBuffer->Unmap(0, 0);
	//m_uploadBuffer->Release();
}


template<typename T>
inline void D3D12UploadBufferResource<T>::CopyData(UINT elementIndex, const T& data)
{
	memcpy(reinterpret_cast<void*>(&m_mappedData[elementIndex * m_elementByteSize]), &data, sizeof(T));
	m_dataBuffer[elementIndex] = data;
}


