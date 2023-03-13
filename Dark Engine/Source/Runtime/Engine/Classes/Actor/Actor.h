#pragma once
#include "../UObject/UObject.h"
#include <DirectXMath.h>
#include <Render/D3D12/D3D12Model.h>
#include <Input/InputCore.h>

using namespace DirectX;


class AActor : public UObject
{
	typedef UObject Super;
public:
	AActor() :	m_position(0, 0, 0),
				m_rotation(0, 0, 0),
				m_scale(1, 1, 1)
	{}

	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;
	virtual void SetupPlayerController(FInputCore* controller);
	void SetMesh(D3D12Mesh* mesh);
	D3D12Model* GetModel() { return m_model.get(); }

public:
	void SetPosition(XMFLOAT3 pos) { m_position = pos; }
	void SetRotation(XMFLOAT3 rot) { m_rotation = rot; }
	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT3 GetFrontVector();
	XMFLOAT3 GetRightVector();
	XMFLOAT3 GetUpVector();



protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;
	std::unique_ptr<D3D12Model> m_model;
	

};