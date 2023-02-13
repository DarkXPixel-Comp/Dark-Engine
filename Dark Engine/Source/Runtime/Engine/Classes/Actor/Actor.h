#pragma once
#include "../UObject/UObject.h"
#include <DirectXMath.h>
#include <Render/D3D12/D3D12Model.h>

using namespace DirectX;


class AActor : public UObject
{
	typedef UObject Super;
public:
	AActor() {}

	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;
	void SetMesh(D3D12Mesh* mesh);
	D3D12Model* GetModel() { return m_model.get(); }


protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;
	std::unique_ptr<D3D12Model> m_model;
	

};