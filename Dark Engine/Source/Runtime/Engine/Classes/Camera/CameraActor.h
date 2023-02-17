#pragma once	 
#include "../Actor/Actor.h"
#include <Render/D3D12/D3D12Camera.h>
#include <Input/InputCore.h>


class UCameraActor : public AActor 
{
	typedef AActor Super;
public:
	UCameraActor()
	{
		
	}
	void OnResize(long, long);

	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;
	void SetupPlayerController(FInputCore* controller) override;
	void MouseLook(float x, float y);
	void W();
	void S();
	void A();
	void D();
	void Space();
	void Action(eInputActionState action);
	D3D12Camera GetCamera() { return m_camera; }

protected:
	D3D12Camera m_camera;
private:
	float sensevity = 0.4;
};