#pragma once
#include "../UObject/UObject.h"
#include <Core.h>
#include <Engine/Classes/Components/ActorComponent/ActorComponent.h>
#include <Components/SceneComponent/USceneComponent.h>
#include <DirectXMath.h>
#include <Render/D3D12/D3D12Model.h>
#include <Input/InputCore.h>

using namespace DirectX;


class DENGINE_API AActor : public UObject
{
	typedef UObject Super;
public:
	AActor() :	m_position(0, 0, 0),
				m_rotation(0, 0, 0),
				m_scale(1, 1, 1)
	{
		m_model = std::make_unique<D3D12Model>();
	}

	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;
	virtual void SetupPlayerController(FInputCore* controller);
	void SetMesh(D3D12Mesh* mesh);
	D3D12Model* GetModel() { return m_model.get(); }

public:
	void SetPosition(XMFLOAT3 pos) { m_position = pos; }
	void SetRotation(XMFLOAT3 rot) { m_rotation = rot; }
	void SetScale(XMFLOAT3 sc) { m_scale = sc; }
	void AddRotation(XMFLOAT3 rot);
	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT3 GetFrontVector();
	XMFLOAT3 GetRightVector();
	XMFLOAT3 GetUpVector();

	void OnUpdateRotation(float delta)
	{
		m_rotation.x += delta * 0.5;
	}



protected:
	//template <typename T>
	//T* CreateDefaultSubObject(FString name);

protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;
	TUniquePtr<D3D12Model> m_model;

	TUniquePtr<USceneComponent> m_rootComponent;


	

};



//template<typename T>
//inline T* AActor::CreateDefaultSubObject(FString name)
//{
//	UActorComponent* component = new T(this);
//	m_ActorComponents.push_back(component);
//	return reinterpret_cast<T*>(component);
//}