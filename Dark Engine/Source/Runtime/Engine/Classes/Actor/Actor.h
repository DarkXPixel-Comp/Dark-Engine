#pragma once
#include <Engine/Classes/UObject/Object.h>

//#include <SimpleMath.h>
#include <DirectXMath.h>
#include <Render/D3D12/D3D12Render.h>


using namespace DirectX;

class AActor : public UObject
{
	typedef UObject Super;
public:
	AActor();


public:
	void BeginPlay() override;
	void Update() override;
	void Destroy() override;

	void AddLocation(XMFLOAT3 loc);
	void AddRotation(XMFLOAT3 rot);
	void AddScaling(XMFLOAT3 scal);

	void SetLocation(XMFLOAT3 loc) { Position = loc; }
	void SetRotation(XMFLOAT3 rot) { Rotation = rot; }

	const XMFLOAT3& GetLocation() const { return Position; }
	const XMFLOAT3& GetRotation() const { return Rotation; };
	const XMFLOAT3& GetScale() const { return Scale; }

	const bool isRendeble() { return visible; }
	D3D12Model* GetModel() { return m_Model; }


	void SetMesh(D3D12Mesh1* mesh);
	void SetModel(D3D12Model* model);


	

protected:
	XMFLOAT3 Position;
	XMFLOAT3 Scale;
	XMFLOAT3 Rotation;


	bool visible = false;

	
protected:
	D3D12Model* m_Model;




};