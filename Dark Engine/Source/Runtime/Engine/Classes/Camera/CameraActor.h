#pragma once	 
#include "../Actor/Actor.h"
#include <Render/D3D12/D3D12Camera.h>


class UCameraActor : public AActor 
{
	typedef AActor Super;
public:
	UCameraActor()
	{
		
	}
	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;
	D3D12Camera GetCamera() { return *m_camera; }

protected:
	std::unique_ptr<D3D12Camera> m_camera;
};